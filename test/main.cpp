

#include "BLObject.h"
#include "BLP2P.h"
#include "BLTimer.h"
#include "argpaser.h"
#include "debug.h"
#include "ssl3/ssl.h"
#include "test/FileClass.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <functional>
#include <linux/BLLinuxTCPSocket.h>
#include <memory>
#include <mutex>
#include <signal.h>
#include <sstream>
#include <stdio.h>
#include <string>
#include <sys/time.h>
#include <thread>
#include <time.h>
#include <tuple>
#include <type_traits>
#include <vector>
#include <sstream>
#include "package.h"
#include "utils/Box.hpp"
#include "filep2p.h"

#define  shell std::cout << GREEN_t <<"wbl:$" << WRITE 
#define  mshell(msg) std::cout << msg


// LOGINSTANCE("default.log")




void server_test(){
    BL::P2P p2p;
    std::atomic<bool> i=true;
    p2p.bind("192.168.1.149", 3308);
    
    BL::P2PTask::ptr tp;
    p2p.isOked.connect(&p2p,[&](BL::P2PTask::ptr p){
        tp=p;
        i=false;
         infoL("ok:%s:%s",p->getIp(),p->getPort());
        p->readReady.connect(p.get(),[](BL::LinuxTCPSocket * s){
            infoL("why?");
            std::vector<ReadBox> re=s->recve();
            infoL("msg:%s",re[0].lenght());
        });
        
    });
     BL::LinuxTimer timer;
    timer.StartWithSecond(3, [&](){
        if(i){
             p2p.connectTo("192.168.1.149", 3309);
        }else{
           // tp->send(std::string("I am server!"));
        }
            
    });
    p2p.start();
}


void client_test(){
    BL::P2P p2p;
    p2p.bind("192.168.1.149", 3309);
    
    std::atomic<bool> i=true;
    BL::P2PTask::ptr tp;
    p2p.isOked.connect(&p2p,[&](BL::P2PTask::ptr p){
        i=false;
        tp=p;
        infoL("ok:%s:%s",p->getIp(),p->getPort());
        p->readReady.connect(p.get(),[](BL::LinuxTCPSocket * s){
            infoL("why?");
            std::vector<ReadBox> re=s->recve();
            infoL("msg:%s",std::string((char *)re[0].unsafe_ptr(),re[0].lenght()));
        });
    });
    BL::LinuxTimer timer;
    timer.StartWithSecond(3, [&](){
        if(i){
            p2p.connectTo("192.168.1.149", 3308);
        }else{
          //  tp->send(std::string("I am client!"));
        }
            
    });
    p2p.start();
}











struct temp_test_data{
    std::ofstream out;
};
BufferToproto buffer;
temp_test_data tso;

bool SaveFile(_file & ffc) {
    _file_FILESTATUS sw = ffc.st_s();
    switch (sw) {
    case _file_FILESTATUS_BEGIN: {
        tso.out.open(ffc.cmd() + "tll");
        tso.out << ffc.data();
        tso.out.flush();
    } break;
    case _file_FILESTATUS_BODY: {
        tso.out << ffc.data();
        tso.out.flush();
    } break;
    case _file_FILESTATUS_END: {
        tso.out << ffc.data();
        tso.out.close();
    }
    default:
        break;
    }

    _file_FILESTATUS se = ffc.st_e();
    if (se == _file_FILESTATUS_END) {
        tso.out.close();
        debugL("file save finshed");
        return true;
    }
    return false;
}

void recaveFile(ReadBox &box) {
    buffer.Push(box);
   
    while (true) {
        proto_package da;
        BufferToproto::BUFFERSTAT st= buffer.GetPackage(da);
        if (st != BufferToproto::BUFFERSTAT::OK) {
            debugL("cant paser");
            return ;
        }
        if (!(da.name() == "_file")) {
            errorL("not a _file");
            return ;
        }
         _file ffc;
        ffc.ParseFromString(da.data());
        bool ret=SaveFile(ffc);
        if(ret){
            return;
        }
    }
}

void file_test_server(){
    BL::P2P p2p;
    p2p.bind("192.168.1.149", 3308);
    BL::P2PTask::ptr pt;
    p2p.isOked.connect(&p2p,[&](BL::P2PTask::ptr p){
        debugL("ok:%s:%s",p->getIp(),p->getPort());
        pt=p;
        p->readReady.connect(p.get(),[](BL::LinuxTCPSocket * s){
            infoL("why?");
            std::vector<ReadBox> re=s->recve();
            for(auto & d:re){
                recaveFile(d);
            }
            
        });
    });
    p2p.start();
}

void file_test_client(){
    Filep2p ff;
    ff.start();
    std::string file;
    sleep(2);
    ff.ConnectTo("192.168.1.149", 3308);
    while(true){
        std::cout  << "input filename" << std::endl;
        std::cin >> file;
        ff.SendFile("CEvent", "192.168.1.149");
    }
}


// void file_test_client(){
//     Filep2p ff;
//     ff.debugCallback=recaveFile;
//     ff.start();
//    // std::string file;
//     sleep(4);
//    // ff.ConnectTo("192.168.1.149", 3308);
//    ff.SendFile("test.file", "192.168.1.149");
//     // while(true){
//     //     std::cout  << "input filename" << std::endl;
//     //     std::cin >> file;
        
//     // }
//     debugL("started");
//     BL::Core core;
//     core.exec();
// }




int main(int argc, char *argv[]) {
    


   
    printf("%sshell~$ %s",GREEN_t,BASECOLOER);
    std::string a;
    std::getline(std::cin,a);
    if(a=="server"){
        SetGLogName("server.log");
        file_test_server();
    }else{
        SetGLogName("client.log");
        file_test_client();
       
    }
    // SetGLogName("test.log");
    // file_test_client();
  

}