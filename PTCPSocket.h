#ifndef __PTCPSOCKET_H_
#define __PTCPSOCKET_H_

#include "PThread.h"

#ifdef _WINDOWS
#else
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>


class PTCPServer;

class PTCPSocket:public PObject{
public:
    PTCPSocket(int fd,PTCPServer * server=nullptr,PObject * parent=nullptr);
    ~PTCPSocket();

    std::string getIp();

    int getPort();

SIGNAL_("read or write")
    /**
     * @brief 当数据来的信号
     * 
     */
    _SIGNAL(PTCPSocket,readReady,
    _ARGS(PTCPSocket * socket),
    _ARGS(PTCPSocket*),
    _ARGS(socket)
    )

    /**
     * @brief 数据发送接口
     * 
     * @param data 数据
     * @param size 大小
     */
    void send(const char * data,int size);

    int read(char * data,int size);

private:
    /**
     * @brief 私有信号
     *  当send被调用时 通过信号通知Socket线程
     *  异步发送避免资源竞态
     * 
     * @param data 
     * @param size 
     */
    _SIGNAL(PTCPSocket,writeReady,
    _ARGS(const char * data,int size),
    _ARGS(const char *,int ),
    _ARGS(data,size)
    )

   
    
private:
    friend PTCPServer;
    struct sendCache_t{
        char * data;
        int size;
    };
    struct sockaddr_in Socketaddr;
    /**
     * @brief writeReady的槽函数
     * 
     * @param data 
     * @param size 
     */
    void onWrite(const char * data,int size);
    std::vector<sendCache_t> dataCache;
    int scfd;
    PTCPServer * m_server=nullptr;//kqueue 管理类
};

class PTCPServer:public PThread{
public:
    PTCPServer(int maxCheckEventSize,PObject * parent=nullptr);

    ~PTCPServer();

    bool bind(const std::string & ip,int port);

    bool listen(int MaxListenSize);

    /**
     * @brief 此函数可能有问题
     * 
     */
    void exit();

private:
   void run()override;

   void accept(struct kevent* ke);

public:
SIGNAL_("connection")
    /**
     * @brief 当链接时发出的信号
     * 
     */
    _SIGNAL(PTCPServer,connected,
    _ARGS(PTCPSocket * socket),
    _ARGS(PTCPSocket *),
    _ARGS(socket)
    )

    /**
     * @brief 
     * 
     * @param socket 
     *  
     */
    _SIGNAL(PTCPServer,errered,
    _ARGS(PTCPSocket * socket),
    _ARGS(PTCPSocket *),
    _ARGS(socket)
    )

    /**
     * @brief 
     * 
     */

    _SIGNAL(PTCPServer,disconnected,
    _ARGS(PTCPSocket * socket),
    _ARGS(PTCPSocket *),
    _ARGS(socket)
    )


private:
    friend PTCPSocket;
    void addEvent(int fd,int event,void *dataPtr=nullptr);

    void deleteEvent(int fd,int event);

    void eableEvent(int fd,int event);

    void disableEvent(int fd,int event);

private:
    int listen_fd=0; ///监听描述符
    int kqueue_handle=0;   //内核队列描述符
    int MaxEventCheckSize=0; //最大的监听事件数量
    struct kevent * checkeventList=nullptr; 
};





#endif


#endif