#ifndef __FILE_P2P
#define __FILE_P2P
#include "BLLinuxTCPSocket.h"
#include "BLObject.h"
#include "BLP2P.h"
#include "BLThread.h"
#include "debug.h"
#include <cstddef>
#include <fstream>
#include <functional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include "datacode.pb.h"
#include "package.h"
#include "utils/Box.hpp"


LOGINSTANCE("client.log")
std::pair<_file,size_t> ReadFile(std::ifstream& os,char * buffer,int bufferlenght,bool & isEnd)
{
    isEnd=false;
   
    _file protoF;
    os.read(buffer, bufferlenght);
    auto end=os.tellg();
    auto le=os.gcount();
     protoF.set_data(std::string(buffer,le));
    if(le!=bufferlenght || le== 0){
        isEnd=true;
    }
    return {protoF,le};
}

class Filep2p:public BL::Thread{
public:
    Filep2p(){
         started.connect(this,this,&Filep2p::SlotRun);
    }
    void SendFileToAll(const std::string & file);
    void GetFile(const std::string &file,const std::string ip);
    void SendFile(const std::string & file,const std::string ip){
        new_send.emit(file,ip);
    }
    void ConnectTo(const std::string & ip,int port){
        if(p2p!=nullptr){
            p2p->connectTo(ip, port);
        }
    }
private:
    struct _send_task{
        BL::LinuxTCPSocket * socket;
       
        std::ifstream in;
        size_t sendedbytes=0;
        std::string fname;
    };
    void SlotRun(){
       p2p=new BL::P2P();
       p2p->isOked.connect(this,this,&Filep2p::SlotOnConnect);
       new_send.connect(this,this,&Filep2p::SlotOnNewSend);
       send_signal.connect(this,this,&Filep2p::SlotOnSendWork);
       p2p->start();
    }
    void SlotOnNewSend(const std::string & f,const std::string &ip){
       auto iter=nodes.find(ip);
       if(iter==nodes.end()){
            errorL("not found ip:%s",ip);
            return;
       }
       _send_task *task=new _send_task;
       task->socket=iter->second.get();
       task->in.open(f);
       if(!task->in.is_open()){
            errorL("open fail!");
            return;
       }
       task->fname=f;
       debugL("open:%s",f);
       sendJob.push_back(task);
       send_signal.emit();
    }
    void SlotOnSendWork(){
        bool isEnd=false;
        char buffer[40960]={0};
        for(auto p=sendJob.begin();p!=sendJob.end();){
             _send_task * t=*p;
             std::pair<_file, size_t> ret= ReadFile(t->in,buffer,40960,isEnd);
            
            if(t->sendedbytes==0){
                // 如果是0 则表表明是文件头
                ret.first.set_st_s(_file_FILESTATUS_BEGIN);
                ret.first.set_cmd(t->fname);
            }else{
                ret.first.set_st_s(_file_FILESTATUS_BODY);
            }
            t->sendedbytes+=ret.second;
            if(isEnd){
                //文件结尾
                ret.first.set_st_e(_file_FILESTATUS_END);
                infoL("send:%s",ret.first.data());
            }
            //组成一个包
            
            ReadBox box=BufferToproto::Serialize(ret.first.SerializeAsString(),"_file");
            t->socket->send(box);
            auto tp=p;
            p++;
            if(isEnd){
                //如果是文件结尾则删除这个任务
               sendJob.erase(tp);
               delete t;
            }
        }
        if(sendJob.size()>0){
            send_signal.emit();
        }
    }

    void SlotOnConnect(BL::P2PTask::ptr socket){
        std::string ip=socket->getIp();
        auto iter=nodes.find(ip);
        if(iter!=nodes.end()){
            errorL("found ip:%s",ip);
            return;
        }
        infoL("connect :%s:%s",ip,socket->getPort());
        nodes[ip]=socket;
        socket->disconnected.connect(this,[this](BL::LinuxTCPSocket * socket){
            auto ite=nodes.find(socket->getIp());
            if(ite!=nodes.end()){
                nodes.erase(ite);
            }
        });

    }
private:
    BL::signal_<> send_signal;
    BL::signal_<const std::string &,const std::string &> new_send;

private:
    BL::P2P * p2p=nullptr;
    std::list<_send_task*> sendJob;
    std::map<std::string ,BL::P2PTask::ptr> nodes;


};


#endif