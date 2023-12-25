#include "PMacTCPSocket.h"
#include "../debug.h"



#ifdef __MACH__

PMacTCPSocket::PMacTCPSocket(int fd,PMacTCPServer * server,Object * parent):Object(parent){
    scfd=fd;
    
    connect(this,&PMacTCPSocket::writeReady,this,&PMacTCPSocket::onWrite);
    if(server==nullptr){
        m_server=new PMacTCPServer(10,this);
    }
    m_server=server;
}

std::string PMacTCPSocket::getIp(){
   // inet_aton(ip.c_str(),&serverAddr.sin_addr);
   return std::string(inet_ntoa(Socketaddr.sin_addr));
}

int PMacTCPSocket::getPort(){
    return ntohs(Socketaddr.sin_port);
}

PMacTCPSocket::~PMacTCPSocket(){
    if(scfd!=0){
        ::close(scfd);
        scfd=0;
    }
}

 void PMacTCPSocket::send(const char * data,int size){
    std::unique_lock<std::mutex> locker(cache_lock);
    sendCaceh.push(data,size);
    m_server->eableEvent(scfd,EVFILT_WRITE,&socketEvent);
    writeReady();
 }

 void PMacTCPSocket::onWrite(){
    std::unique_lock<std::mutex> locker(cache_lock);
    char buffer[4096]={0};
    int ret=0;
    while(sendCaceh.size()>0){
       ret=sendCaceh.pop(buffer,4096);
       ::send(scfd,buffer,ret ,0);
    }
 }

int PMacTCPSocket::read(char * data,int size){
    int ret= ::read(scfd,data,size);
    if(ret==0){
        m_server->deleteEvent(scfd,EVFILT_READ,&socketEvent);
        ::close(scfd);
        scfd=0;
    }
    return ret;
}

PMacTCPServer::PMacTCPServer(int maxCheckEventSize,Object * parent):Thread(parent){
    kqueue_handle=kqueue();
    checkeventList=new struct kevent[maxCheckEventSize];
    MaxEventCheckSize=maxCheckEventSize;
}

PMacTCPServer::~PMacTCPServer(){
    if(listen_fd!=0){
        ::close(listen_fd);
    }
    listen_fd=0;
}


bool PMacTCPServer::bind(const std::string & ip,int port){
    int ret=0;
    listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   
    if (listen_fd < 0){
         errorL("socket:" << strerror(errno));
         return false;
    }

    sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_aton(ip.c_str(),&serverAddr.sin_addr);

    ret = ::bind(listen_fd, (sockaddr *)&serverAddr, sizeof(serverAddr));
    if (0> ret){
        errorL("bind:" << strerror(errno));
        return false;
    }
    return true;
}

bool PMacTCPServer::listen(int MaxListenSize){
    int ret= ::listen(listen_fd, MaxListenSize);
    if(ret< 0){
        errorL("listen:" << strerror(errno));
        return false;
    }
    addEvent(listen_fd, EVFILT_READ|EVFILT_WRITE,&listen_event);
    
    return true;
}

void PMacTCPServer::run(){
    int ret = kevent(kqueue_handle, NULL, 0, checkeventList, MaxEventCheckSize, NULL);
    if (ret < 0)
    {
        errorL("kevent:" << strerror(errno));
        return;
    }

    for (int i = 0; i < ret; i++)
        {
            int fd = checkeventList[i].ident;
            int size = checkeventList[i].data;
            
            if (listen_fd == fd)
            {
                accept(&checkeventList[i]);
            }
            else if (checkeventList[i].filter == EVFILT_READ)
            {
                void * socketPtr=checkeventList[i].udata;
                if(socketPtr==nullptr){
                    errorL("EVFILT_READ: no PTCPSocet pointer");
                    ::sleep(1000);
                    continue;
                }
                PMacTCPSocket * Psocket=static_cast<PMacTCPSocket*>(socketPtr);
                Psocket->readReady(Psocket);

            }

            else if(checkeventList[i].filter == EVFILT_WRITE)
            {
               infoL("has writed !!!");
               struct kevent ne;
               disableEvent(fd,EVFILT_WRITE,&ne);
            }
            else
            {
               errorL("kevent:" << strerror(errno));
            }
        }
    started();    
}

void PMacTCPServer::exit(){
    EventLoop::exit();
    deleteEvent(listen_fd,EVFILT_READ|EVFILT_WRITE,&listen_event);
    
}

void PMacTCPServer::accept(struct kevent * ke){
    int eventSize=ke->data;
    int eventFd=ke->ident;

    for(int i=0;i< eventSize;i++){
        sockaddr_in clientAddr;
        socklen_t lent = sizeof(sockaddr_in);
        int client_fd = ::accept(eventFd, (sockaddr *)&clientAddr, &lent);
        if(client_fd <0){
            errorL("accpet:" << strerror(errno));
            continue;
        }
        PMacTCPSocket * client_socket=new PMacTCPSocket(client_fd,this,this);
        client_socket->Socketaddr=clientAddr;
        addEvent(client_fd,EVFILT_READ|EVFILT_WRITE,&client_socket->socketEvent,(void *)client_socket);
        connected(client_socket);
    }
   

}


void PMacTCPServer::addEvent(int fd,int event, struct kevent *e,void *dataPtr){
    int ret = 0;
   
    EV_SET(e, fd, event, EV_ADD, 0, 0, dataPtr);
    ret = kevent(kqueue_handle, e, 1, NULL, 0, NULL);
    if (ret < 0){
         errorL("addEvent:" << strerror(errno));
    }
}

void PMacTCPServer::deleteEvent(int fd,int event, struct kevent *e){
    int ret = 0;
    
    EV_SET(e, fd, event, EV_DELETE, 0, 0, NULL);
    ret = kevent(kqueue_handle, e, 1, NULL, 0, NULL);
   if (ret < 0){
         errorL("deleteEvent:" << strerror(errno));
    }
}

void PMacTCPServer::eableEvent(int fd,int event, struct kevent *e){
    int ret = 0;
   // debugL("file description:" << fd);
    EV_SET(e, fd, event, EV_ADD|EV_ENABLE, 0, 0, NULL);
    ret = kevent(kqueue_handle, e, 1, NULL, 0, NULL);
    if (ret < 0){
         errorL("eableEvent:" << strerror(errno));
         
    }
    debugL("ret:" << fd);
}

void PMacTCPServer::disableEvent(int fd,int event, struct kevent *e){

    int ret = 0;
    EV_SET(e, fd, event, EV_DISABLE, 0, 0, NULL);
    ret = kevent(kqueue_handle, e, 1, NULL, 0, NULL);
    if (ret < 0){
         errorL("disableEvent:" << strerror(errno));
    }
}



#endif


