#ifndef __LinuxTCPSocket_H_LINUX
#define __LinuxTCPSocket_H_LINUX
#include "../BLThread.h"
#include "../PByteArray.hpp"
#include "BLObject.h"
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>

#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <queue>
#include "utils/Box.hpp"


namespace BL {


class EPOLLObject:public Object,public std::enable_shared_from_this<EPOLLObject>{
    public:

    EPOLLObject(int maxEventSize);
    ~EPOLLObject();

    struct DataPackge{
        void * dataPtr;
        std::function<void(void *)> IN_callback;
        std::function<void(void *)> OUT_callback;
        std::function<void(void *)> ERROR_callback;
    };

    
    signal_<int ,struct epoll_event * > IOEvent;

    signal_<int ,struct epoll_event *> IOClosed;
  
    void Add(int fd, int event, struct epoll_event *e, DataPackge *dataPtr);

    void Mod(int fd, int event, struct epoll_event *e, DataPackge *dataPtr);

    void Delete(int fd, int event, struct epoll_event *e, DataPackge *dataPtr);

private:



    void run();

    int epoll_fd = 0;          // epoll描述符
    int MaxEventCheckSize = 0; // 最大的监听事件数量
    std::thread _th;
    struct epoll_event *checkEventlist=nullptr;
};

class LinuxTCPServer;

class LinuxTCPSocket : public Object
{
public:
    LinuxTCPSocket(EPOLLObject * eobj);
    LinuxTCPSocket(const std::thread::id & id,int fd,EPOLLObject * eobj);
    ~LinuxTCPSocket();

    std::string getIp();

    int getPort();

    int connectToHost(const std::string & ip,int port);

   
  
    signal_<LinuxTCPSocket *> readReady;

    signal_<LinuxTCPSocket *> disconnected;

    void send(ReadBox data);

    void sendNothing();
    std::vector<ReadBox> recve();
    void close();
    struct epoll_event socketEvent;
private:
    
    signal_<>  writeReady;

private:
    friend LinuxTCPServer;
    friend EPOLLObject::DataPackge;
    struct sockaddr_in Socketaddr;
   
    void onSend(void *arg);
    void onRecve(void *arg);
    int bindEPOLL();
    std::queue<ReadBox> sendCaceh;
    std::queue<ReadBox> recaveCaceh;
    std::mutex send_cache_lock;
    std::mutex recve_cache_lock;
    
    int scfd=-1;
    EPOLLObject * epoll_obj=nullptr;
    EPOLLObject::DataPackge * thispackage=nullptr;
    //std::atomic<bool> isOk=false;
};

class LinuxTCPServer : public Object
{
public:
    LinuxTCPServer(EPOLLObject * eobj);

    ~LinuxTCPServer();

    bool bind(const std::string &ip, int port);

    bool listen(int MaxListenSize);

   


    void start();

    /**
     * @brief 此函数可能有问题
     *
     */
    void exit();

private:
    void run() ;

    void accept(void* arg);



public:
   
   signal_<LinuxTCPSocket *> connected;
          
   signal_<LinuxTCPSocket* >    errered;

private:
    friend LinuxTCPSocket;

    

private:
    EPOLLObject * epoll_obj=nullptr;
    EPOLLObject::DataPackge * thispackage=nullptr;
    int listen_fd = 0;
    struct epoll_event listen_event;
};





}





#endif

#endif
