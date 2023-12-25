#ifndef __PMacTCPSocket_H_MACH
#define __PMacTCPSocket_H_MACH

#include "../BLThread.h"
#include "../PByteArray.hpp"




#ifdef __MACH__
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "PByteArray.hpp"

class PMacTCPServer;

class PMacTCPSocket : public Object
{
public:
    PMacTCPSocket(int fd, PMacTCPServer *server = nullptr, Object *parent = nullptr);
    ~PMacTCPSocket();

    std::string getIp();

    int getPort();

    SIGNAL_("read or write")
    /**
     * @brief 当数据来的信号
     *
     */
    _SIGNAL(PMacTCPSocket, readReady,
            _ARGS(PMacTCPSocket *socket),
            _ARGS(PMacTCPSocket *),
            _ARGS(socket))

    /**
     * @brief 数据发送接口
     *
     * @param data 数据
     * @param size 大小
     */
    void send(const char *data, int size);

    int read(char *data, int size);
    struct kevent socketEvent;

private:
    /**
     * @brief 私有信号
     *  当send被调用时 通过信号通知Socket线程
     *  异步发送避免资源竞态
     *
     * @param data
     * @param size
     */
    _SIGNAL_VOID(PMacTCPSocket, writeReady)

private:
    friend PMacTCPServer;
    struct sockaddr_in Socketaddr;
    /**
     * @brief writeReady的槽函数
     *
     * @param data
     * @param size
     */
    void onWrite();
    PByteArray sendCaceh;
    PByteArray recaveCaceh;
    int scfd;
    PMacTCPServer *m_server = nullptr; // kqueue 管理类
};

class PMacTCPServer : public Thread
{
public:
    PMacTCPServer(int maxCheckEventSize, Object *parent = nullptr);

    ~PMacTCPServer();

    bool bind(const std::string &ip, int port);

    bool listen(int MaxListenSize);

    /**
     * @brief 此函数可能有问题
     *
     */
    void exit();

private:
    void run() override;

    void accept(struct kevent *ke);

public:
    SIGNAL_("connection")
    /**
     * @brief 当链接时发出的信号
     *
     */
    _SIGNAL(PMacTCPServer, connected,
            _ARGS(PMacTCPSocket *socket),
            _ARGS(PMacTCPSocket *),
            _ARGS(socket))

    /**
     * @brief
     *
     * @param socket
     *
     */
    _SIGNAL(PMacTCPServer, errered,
            _ARGS(PMacTCPSocket *socket),
            _ARGS(PMacTCPSocket *),
            _ARGS(socket))

    /**
     * @brief
     *
     */

    _SIGNAL(PMacTCPServer, disconnected,
            _ARGS(PMacTCPSocket *socket),
            _ARGS(PMacTCPSocket *),
            _ARGS(socket))

private:
    friend PMacTCPSocket;
    void addEvent(int fd, int event, struct kevent *e, void *dataPtr = nullptr);

    void deleteEvent(int fd, int event, struct kevent *e);

    void eableEvent(int fd, int event, struct kevent *e);

    void disableEvent(int fd, int event, struct kevent *e);

private:
    int listen_fd = 0;         /// 监听描述符
    int kqueue_handle = 0;     // 内核队列描述符
    int MaxEventCheckSize = 0; // 最大的监听事件数量
    struct kevent listen_event;
    struct kevent *checkeventList = nullptr;
};

#endif

#endif