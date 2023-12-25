#ifndef _P2P_NODE_H
#define _P2P_NODE_H
#include "BLLinuxTCPSocket.h"
#include "BLObject.h"
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include "BLTimer.h"
#include "debug.h"
#include "utils/Box.hpp"
#include <random>
#include <vector>
namespace BL{
class P2PTask{
public:
    enum st{
        REQ,
        ACK,
        OKSERVERREQ,
        OKCLIENTREQ,
        OKSERVERACK,
        OKCLIENTACK
    };
    using ptr=std::shared_ptr<LinuxTCPSocket>;
    P2PTask(const std::function<void(ptr socket)> & cb);
    int connect(const std::string & ip,int port,Object *ob,EPOLLObject * eobj);
    void SetSeverSocket(LinuxTCPSocket * s);
    int  GetSocketTimeout();
private:
    
    struct shpk{
        uint8_t s_t;
        uint64_t num;
    };
    void SlotRead(LinuxTCPSocket* s);

    void SendSelf(LinuxTCPSocket *sc,st s,uint64_t num);
    void  check();
   
    ptr client=nullptr;
    ptr server=nullptr;
    uint64_t selfnum;
    uint64_t othernum;
    std::atomic<int> _time;
    std::function<void(ptr socket)> OnShakFinshed;
    std::atomic<bool> checked=false;
    
   
};


class P2P:public BL::Object{
public:
    using taksPtr=std::shared_ptr<BL::P2PTask>;
    P2P();
    void bind(const std::string & ip,int port);
    void start();
    void connectTo(const std::string & ip,int port);
    BL::signal_<BL::P2PTask::ptr> isOked;
private:
    void EmitPtr(BL::P2PTask::ptr p);
    void SlotConnect(const std::string &  ip,int port);
    taksPtr Find(const std::string & ip);
    BL::signal_<const std::string &,int> toC;
    BL::signal_<> toChekc;
    
    BL::LinuxTCPServer * server;
    BL::EPOLLObject * obeject;
    std::map<std::string,taksPtr> tMap;
    std::mutex lock;
    BL::LinuxTimer timer;
};



}

#endif