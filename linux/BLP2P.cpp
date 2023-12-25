#include "BLP2P.h"
#include "BLLinuxTCPSocket.h"
#include "debug.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <vector>

LOGINSTANCE("p2p.log")

namespace BL{

 P2PTask::P2PTask(const std::function<void(ptr socket)> & cb){
        _time=10;
        std::random_device seed;
	 	std::ranlux48 engine(seed());
	 	std::uniform_int_distribution<int> u(0, 200);
	 	selfnum= u(engine);
        OnShakFinshed=cb;

    }
    int P2PTask::connect(const std::string & ip,int port,Object *ob,EPOLLObject * eobj){
        if(client!=nullptr){
            return -1;
        }
        LinuxTCPSocket * ts=new LinuxTCPSocket(ob->getWorkThreadLoopId(),-1,eobj);
        int re=ts->connectToHost(ip, port);
        if(re!=0){
            delete ts;
            return re;
        }
        client=ptr(ts);
        client->readReady.connect(ob,this,&P2PTask::SlotRead);
        SendSelf(client.get(), st::REQ, selfnum);
        return 0;
    }
    void P2PTask::SetSeverSocket(LinuxTCPSocket * s){
        if(server!=nullptr){
            delete s;
            return;
        }
        server=ptr(s);
        s->readReady.connect(s,this,&P2PTask::SlotRead);
    }
   

    int  P2PTask::GetSocketTimeout(){
        return --_time;
    }


       void P2PTask::SlotRead(LinuxTCPSocket* s){
      
       shpk sp;
       std::vector<ReadBox> re=s->recve();


       if(re.size()==0){
            errorL("recave 0");
            return;
       }
       auto in =re[0].CreateIndex();
       if(re[0].lenght()<sizeof(shpk)){
            errorL("read size less shpk %s",re[0].lenght());
            return;
       }
       in.read((unsigned char *)&sp,sizeof(shpk));
       st cmd=(st)sp.s_t;
       if(cmd==ACK){
            check();
            othernum=sp.num;
       }
       if(cmd==REQ){
            check();
            othernum=sp.num;
            SendSelf(s, ACK, selfnum);
       }
       if(cmd==OKSERVERREQ){
            SendSelf(server.get(), OKCLIENTACK, 100);
            server->readReady.Clear();
            OnShakFinshed(server);
            
       }
       if(cmd==OKCLIENTREQ){
            SendSelf(client.get(),OKSERVERACK, 100);
            client->readReady.Clear();
            OnShakFinshed(client);
            
       }
       if(cmd==OKSERVERACK){
            server->readReady.Clear();
            OnShakFinshed(server);
           
       }
       if(cmd==OKCLIENTACK){
            client->readReady.Clear();
            OnShakFinshed(client);
            
       }
    }

    void P2PTask::SendSelf(LinuxTCPSocket *sc,st s,uint64_t num){
        shpk *sp=new shpk;
        sp->s_t=s;
        sp->num=num;
        sc->send(ReadBox((unsigned char *)sp,sizeof(shpk)));
    }
    void  P2PTask::check(){
       if(selfnum > othernum){
            if(checked==true) return;
            checked=true;
            if(client!=nullptr){
                SendSelf(client.get(), OKSERVERREQ, 100);
                return ;
            }
            if(server!=nullptr){
                SendSelf(server.get(), OKCLIENTREQ, 100);
                return ;
           }
       }
    }


    P2P::P2P(){
         obeject=new BL::EPOLLObject(30);
         server=new BL::LinuxTCPServer(obeject);
         toC.connect(this,this,&P2P::SlotConnect);
         toChekc.connect(this,[this](){
                auto iter=tMap.begin();
                int __time=0;
                for(;iter!=tMap.end();){
                    __time=iter->second->GetSocketTimeout();
                //   debugL("check time %s",__time);
                    if(__time<0){
                       // std::cout << Sutil::Format("time out") << std::endl;
                        iter=tMap.erase(iter);
                        continue;
                    }
                   iter++; 
                }
         });
         server->connected.connect(this,[this](BL::LinuxTCPSocket *s){
             auto ts=Find(s->getIp());
             if(ts!=nullptr){
                ts->SetSeverSocket(s);
             }
         });

         
         
    }

    void P2P::bind(const std::string & ip,int port){
        server->bind(ip, port);
        server->listen(30);
    }
    void P2P::start(){
        timer.StartWithSecond(1, [this](){
                toChekc.emit();
         });
        server->start();
    }

    void P2P::connectTo(const std::string & ip,int port){
        toC.emit(ip,port);
    }

    void P2P::EmitPtr(BL::P2PTask::ptr p){
        isOked.emit(p);
        debugL("emit %s:%s",p->getIp(),p->getPort());
    }
    void P2P::SlotConnect(const std::string &  ip,int port){
        taksPtr p=Find(ip);
        p->connect(ip, port, this, obeject);
    }

    P2P::taksPtr P2P::Find(const std::string & ip){
        std::unique_lock<std::mutex> locker(lock);
        auto iter=tMap.find(ip);
        if(iter==tMap.end()){
            taksPtr pt=taksPtr(new BL::P2PTask(std::bind(&P2P::EmitPtr,this,std::placeholders::_1)));
            tMap[ip]=pt;
            return pt;
        }else{
            return tMap[ip];
        }
    }
}