#include "BLLinuxTCPSocket.h"
#include "BLEventLoop.h"
#include "BLObject.h"
#include <asm-generic/errno-base.h>
#include <cerrno>
#include <cstring>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <vector>
#ifdef __linux__
#include "../debug.h"

LOGINSTANCE("socket.log")
namespace BL {

LinuxTCPSocket::LinuxTCPSocket(EPOLLObject *eboj) {
    epoll_obj = eboj;
    thispackage = new EPOLLObject::DataPackge;
    thispackage->dataPtr = this;
    thispackage->IN_callback = Bind(&LinuxTCPSocket::onRecve, this);
    thispackage->OUT_callback = Bind(&LinuxTCPSocket::onSend, this);
}

LinuxTCPSocket::LinuxTCPSocket(const std::thread::id &id, int fd,
                               EPOLLObject *eobj) {
    thispackage = new EPOLLObject::DataPackge;
    thispackage->dataPtr = this;
    thispackage->IN_callback = Bind(&LinuxTCPSocket::onRecve, this);
    thispackage->OUT_callback = Bind(&LinuxTCPSocket::onSend, this);
    scfd = fd;
    epoll_obj = eobj;
    setObjectThread(id);
}

int LinuxTCPSocket::connectToHost(const std::string & ip,int port){
    int ret = 0;
    scfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (scfd < 0) {
        errorL("socket: %s", strerror(errno));
        return scfd;
    }
    Socketaddr.sin_family = AF_INET;
    Socketaddr.sin_port = htons(port);
    inet_aton(ip.c_str(), &Socketaddr.sin_addr);

    ret=::connect(scfd, (const struct sockaddr *)&Socketaddr, sizeof(sockaddr_in));

    if(ret!=0){
        errorL("connect fail: %s" , strerror(errno));
        scfd=-1;
        return ret;
    }
    epoll_obj->Add(scfd, EPOLLIN, &socketEvent, thispackage);
    return 0;

}

LinuxTCPSocket::~LinuxTCPSocket() {
    if (scfd > 0) {
        debugL("socket %s:%s has self closed",getIp(),getPort());
        ::close(scfd);
        delete thispackage;
        scfd = -1;
    }
    
}

void LinuxTCPSocket::onRecve(void *arg) {
   unsigned  char buffer[40960]={0};
    int recvbytes = 0;
    std::unique_lock<std::mutex> locker(recve_cache_lock);
    if (scfd < 0) {
        errorL("socket:%s closed" , getIp() );
        return;
    }

    while (true) {
        recvbytes = ::recv(scfd, buffer, 40960, MSG_DONTWAIT);
        if (recvbytes == 0) {
            //链接断开
            close();
            debugL("disconnect: %s:%s", getIp(),getPort());
            disconnected.emit(this);
            //epoll_obj->Delete(scfd, EPOLLIN, &socketEvent, thispackage);
            
            return ;
        } else if (recvbytes < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN || errno == EINTR) {
               // debugL("what:%s",strerror(errno));
                break;
            }
            close();
            errorL("%s:%s",errno,strerror(errno));
            disconnected.emit(this);
            //epoll_obj->Delete(scfd, EPOLLIN, &socketEvent, thispackage);
           
            return;
        }
        recaveCaceh.push(ReadBox(buffer, recvbytes));
    }
    readReady.emit(this);
    //debugL("this is why!!!");
}

std::string LinuxTCPSocket::getIp() {
    return std::string(inet_ntoa(Socketaddr.sin_addr));
}

int LinuxTCPSocket::getPort() { return ntohs(Socketaddr.sin_port); }

void LinuxTCPSocket::send(ReadBox box) {
    if (scfd < 0 ) {
        errorL("socket:%s closed" , getIp() );
        return;
    }
    std::unique_lock<std::mutex> locker(send_cache_lock);
    sendCaceh.push(box);
    epoll_obj->Mod(scfd, EPOLLOUT, &socketEvent, thispackage);
}

void LinuxTCPSocket::sendNothing(){
    epoll_obj->Mod(scfd, EPOLLOUT, &socketEvent, thispackage);
}

std::vector<ReadBox> LinuxTCPSocket::recve() {
    std::unique_lock<std::mutex> locker(recve_cache_lock);
    std::vector<ReadBox> ret;
    while (recaveCaceh.size() > 0) {
        ret.push_back( recaveCaceh.front());
        recaveCaceh.pop();
    }
    return ret;
}

int LinuxTCPSocket::bindEPOLL(){
    epoll_obj->Add(scfd, EPOLLIN, &socketEvent, thispackage);
    return 0;
}

void LinuxTCPSocket::onSend(void *arg) {
    if (scfd == -1) {
        errorL("socket:%s closed",getIp());
        return;
    }
    std::unique_lock<std::mutex> locker(send_cache_lock);

    int sendBytes = 0;
    while (sendCaceh.size() > 0) {

         ReadBox box = sendCaceh.front();
          
        sendBytes = ::send(scfd, box.unsafe_ptr(), box.lenght(), 0);
        if (sendBytes < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN || errno == EINTR) {
                return;
            }
             close();
            errorL("send:%s" , strerror(errno));
            disconnected.emit(this);
           
            return;
        } else if (sendBytes == 0) {
            close();
            disconnected.emit(this);
           
            return;
        }
        sendCaceh.pop();
    }
    epoll_obj->Mod(scfd, EPOLLIN, &socketEvent, thispackage);
    return;
}

void LinuxTCPSocket::close() {
    if (scfd > 0) {
        ::close(scfd);
       // epoll_obj->Delete(scfd, EPOLLIN, &socketEvent, thispackage);
        delete thispackage;
        scfd = -1;
    }
}

LinuxTCPServer::LinuxTCPServer(EPOLLObject *eobj) {
    epoll_obj = eobj;

    thispackage = new EPOLLObject::DataPackge;

    thispackage->dataPtr = this;
    thispackage->IN_callback = Bind(&LinuxTCPServer::accept, this);
}

LinuxTCPServer::~LinuxTCPServer() {
    if (listen_fd > 0) {
        ::close(listen_fd);
        listen_fd = -1;
        delete thispackage;
    }
    
    
}

bool LinuxTCPServer::bind(const std::string &ip, int port) {
    int ret = 0;
    listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listen_fd < 0) {
        errorL("socket:%s", strerror(errno));
        return false;
    }

    sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_aton(ip.c_str(), &serverAddr.sin_addr);

    int opt = 1;
	setsockopt( listen_fd, SOL_SOCKET,SO_REUSEADDR, 

					(const void *)&opt, sizeof(opt) );

    ret = ::bind(listen_fd, (sockaddr *)&serverAddr, sizeof(sockaddr_in));
    if (0 > ret) {
        errorL("[%s:%s]->bind: %s",ip,port, strerror(errno));
        return false;
    }
    return true;
}

bool LinuxTCPServer::listen(int MaxListenSize) {
    int ret = ::listen(listen_fd, MaxListenSize);
    if (ret < 0) {
        errorL("listen: %s",strerror(errno));
        return false;
    }
    epoll_obj->Add(listen_fd, EPOLLIN, &listen_event, thispackage);
    return true;
}



void LinuxTCPServer::accept(void *arg) {

    sockaddr_in clientAddr;
    socklen_t lent = sizeof(sockaddr_in);
    int client_fd = ::accept(listen_fd, (sockaddr *)&clientAddr, &lent);
    if (client_fd < 0) {
        errorL("accpet:%s" ,strerror(errno));
    }
    

    LinuxTCPSocket *client_socket =
        new LinuxTCPSocket(getWorkThreadLoopId(), client_fd, epoll_obj);
     client_socket->Socketaddr = clientAddr;

    debugL("accept: %s",client_socket->getIp());
    client_socket->bindEPOLL();
    connected.emit(true,client_socket);
    
}
void LinuxTCPServer::start(){
   EventLoop::exec();
}



EPOLLObject::EPOLLObject(int maxEventSize) {
    epoll_fd = epoll_create(maxEventSize);
    if (epoll_fd < 0) {
        errorL("epoll_create:%s" ,strerror(errno));
        return;
    }
    this->MaxEventCheckSize = maxEventSize;
    checkEventlist = new struct epoll_event[maxEventSize];
    _th=std::thread(&EPOLLObject::run,this);
    _th.detach();
}

EPOLLObject::~EPOLLObject() {
    if (close(epoll_fd)) {
        errorL("close:%s",strerror(errno));
    }
    delete [] checkEventlist;
}

void EPOLLObject::Add(int fd, int event, struct epoll_event *e,
                      DataPackge *dataPtr) {
    e->events = event;
    e->data.ptr = (void *)dataPtr;
    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, e);
    if (ret < 0) {
        errorL("epoll_ctl:EPOLL_CTL_ADD:%s", strerror(errno));
    }
}

void EPOLLObject::Mod(int fd, int event, struct epoll_event *e,
                      DataPackge *dataPtr) {
    e->events = event;
    e->data.ptr = (void *)dataPtr;
    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, e);
    if (ret < 0) {
        errorL("epoll_ctl:EPOLL_CTL_MOD:%s", strerror(errno));
    }
}

void EPOLLObject::Delete(int fd, int event, struct epoll_event *e,
                         DataPackge *dataPtr) {
    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, e);
    if (ret < 0) {
        errorL("epoll_ctl:EPOLL_CTL_DEL:%s" , strerror(errno));
    }
}

void EPOLLObject::run() {
    int ret = 0;
   // debugL("run.....");
    while (true) {

        ret = epoll_wait(epoll_fd, checkEventlist, MaxEventCheckSize - 1, -1);
        if (ret <= 0) {
            errorL("epoll_wait:%s",strerror(errno));
            return ;
        }
        for (int i = 0; i < ret; i++) {
            int event_ = checkEventlist[i].events;

            DataPackge *cli =static_cast<DataPackge *>(checkEventlist[i].data.ptr);

            if (event_ & EPOLLIN) {
                if (cli->IN_callback) {
                  //  debugL("in ....");
                    cli->IN_callback(cli);
                    
                }
            } else if (event_ & EPOLLOUT) {
                if (cli->OUT_callback) {

                    cli->OUT_callback(cli);
                }
            } else if (event_ & EPOLLERR) {
                if (cli->ERROR_callback) {
                    cli->ERROR_callback(cli);
                }
                errorL("EPOLL what:%s" , event_);
            }
        }
    }
}

} // namespace BL

#endif
