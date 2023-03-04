#include "PTCPSocket.h"
#include "debug.h"



#ifdef _WINDOWS

#elif __linux__
PTCPSocket::PTCPSocket(int fd, PTCPServer* server, PObject* parent) {
    scfd = fd;
    if (server == nullptr) {
        m_server = new PTCPServer(10, this);
    }
    connect(this,&PTCPSocket::writeReady,this,&PTCPSocket::onWrite);
    m_server = server;
    isOk=true;
}


PTCPSocket::~PTCPSocket() {
    if (scfd > 0) {
        ::close(scfd);
        scfd = -1;
    }
}


std::string PTCPSocket::getIp() {
     return std::string(inet_ntoa(Socketaddr.sin_addr));
}

int PTCPSocket::getPort() {
    return ntohs(Socketaddr.sin_port);
}

void PTCPSocket::send(const char* data, int size)
{
    if(scfd==-1|| isOk==false){
        return;
    }
    std::unique_lock<std::mutex> locker(cache_lock);
    sendCaceh.push(data, size);
    m_server->Mod(scfd, EPOLLOUT,&socketEvent,this);
    writeReady();
}

int PTCPSocket::read(char* data, int size)
{
    if(isOk==false){
        return 0;
    }
    int ret=::read(scfd,data,size);

    if(ret<0){
        if(errno==EAGAIN){
            return ret;
        }else{
            isOk=false;
            m_server->Mod(scfd,EPOLLOUT,&socketEvent,this);
            m_server->freeSocket(this);
           
            return ret;
        }
    }else if(ret==0){
        isOk=false;
        m_server->Mod(scfd,EPOLLOUT,&socketEvent,this);
        disconnected(this);
        m_server->freeSocket(this);
    }
    return ret;
}

void PTCPSocket::onWrite(){
    if(scfd==-1){
        return ;
    }
    std::unique_lock<std::mutex> locker(cache_lock);
    char buffer[4096]={0};
    int ret=0;
    while(sendCaceh.size()>0){
       ret=sendCaceh.pop(buffer,4096);
       ret=::send(scfd,buffer,ret ,0);
       if(ret <0){
            if(errno==EAGAIN){
                return ;
            }else{
                isOk=false;
                m_server->Mod(scfd,EPOLLOUT,&socketEvent,this);
                disconnected(this);
                m_server->freeSocket(this);
                return ;
            }
       }
    }
    if(ret >0){
        m_server->Mod(scfd,EPOLLIN,&socketEvent,this);
    }
    return ;
 }

void PTCPSocket::close()
{
    if(scfd>0){
        ::close(scfd);
        scfd=-1;
    }
}

PTCPServer::PTCPServer(int maxCheckEventSize, PObject* parent)
{
    epoll_fd = epoll_create(maxCheckEventSize);
    if (epoll_fd < 0) {
        errorL("epoll_create:" << strerror(errno));
    }
    this->MaxEventCheckSize = maxCheckEventSize;
    checkEventlist = new struct epoll_event[maxCheckEventSize];

    connect(this,&PTCPServer::freeSocket,this,&PTCPServer::onFreeSocketSlot);
}

PTCPServer::~PTCPServer()
{
    if (listen_fd != 0) {
        ::close(listen_fd);
    }
    listen_fd = 0;
}

bool PTCPServer::bind(const std::string& ip, int port)
{
    int ret = 0;
    listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listen_fd < 0) {
        errorL("socket:" << strerror(errno));
        return false;
    }

    sockaddr_in serverAddr = { 0 };
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_aton(ip.c_str(), &serverAddr.sin_addr);

    ret = ::bind(listen_fd, (sockaddr*)&serverAddr, sizeof(sockaddr_in));
    if (0 > ret) {
        errorL("bind:" << strerror(errno));
        return false;
    }
    return true;
}

bool PTCPServer::listen(int MaxListenSize)
{
    int ret = ::listen(listen_fd, MaxListenSize);
    if (ret < 0) {
        errorL("listen:" << strerror(errno));
        return false;
    }
    PTCPSocket* listen_Socket = new PTCPSocket(listen_fd, this, this);
    Add(listen_fd,EPOLLIN, &listen_event, listen_Socket);

    return true;
}

void PTCPServer::exit()
{
    PEventLoop::exit();
    Delete(listen_fd, 0,&listen_event, &listen_Socket);
}

void PTCPServer::run()
{
    int ret = 0;
    debugL("run.....");
    ret = epoll_wait(epoll_fd, checkEventlist, MaxEventCheckSize - 1, -1);
    if (ret <= 0) {
        errorL("epoll_wait:" << strerror(errno));
    }
    for (int i = 0; i < ret; i++) {
        int event_ = checkEventlist[i].events;
        PTCPSocket* cli = static_cast<PTCPSocket*>(checkEventlist[i].data.ptr);
        if (cli == nullptr) {
            errorL("EPOLL: no PTCPSocet pointer");
            continue;
        }
        if (cli->scfd == listen_fd) {
            this->accept(&cli->socketEvent);
        }
        else if(event_& EPOLLIN) {
            cli->readReady(cli);
        }
        else if (event_ & EPOLLOUT) {
            debugL("EPOLLOUT");
        }
        else {
            errorL("EPOLL what:" << event_);
        }
    }
    started();
}

void PTCPServer::accept(struct epoll_event* e) {

    sockaddr_in clientAddr;
    socklen_t lent = sizeof(sockaddr_in);
    int client_fd = ::accept(listen_fd, (sockaddr*)&clientAddr, &lent);
    if (client_fd < 0) {
        errorL("accpet:" << strerror(errno));
    }
    PTCPSocket* client_socket = new PTCPSocket(client_fd, this, this);
    client_socket->Socketaddr = clientAddr;
    Add(client_fd, EPOLLIN, &client_socket->socketEvent,(void *)client_socket);
    connected(client_socket);
}

void PTCPServer::onFreeSocketSlot(PTCPSocket *socket)
{
    debugL("On Free");
    Delete(socket->scfd,EPOLLOUT|EPOLLIN,&socket->socketEvent,socket);
    socket->close();
    delete socket;
}

void PTCPServer::Add(int fd, int event, struct epoll_event *e, void *dataPtr)
{
    e->events= event;
    e->data.ptr = dataPtr;
    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, e);
    if (ret <0) {
        errorL("epoll_ctl:EPOLL_CTL_ADD:" << strerror(errno));
    }
}

void PTCPServer::Mod(int fd, int event, struct epoll_event* e, void* dataPtr)
{
    e->events= event;
    e->data.ptr = dataPtr;
    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, e);
    if (ret <0) {
        errorL("epoll_ctl:EPOLL_CTL_MOD" << strerror(errno));
    }
}

void PTCPServer::Delete(int fd, int event, struct epoll_event* e, void* dataPtr)
{
    int ret = epoll_ctl(epoll_fd, EPOLL_CTL_DEL,fd, e);
    if (ret < 0) {
        errorL("epoll_ctl:EPOLL_CTL_DEL" << strerror(errno));
    }
}



#elif __MACH__

PTCPSocket::PTCPSocket(int fd,PTCPServer * server,PObject * parent):PObject(parent){
    scfd=fd;
    
    connect(this,&PTCPSocket::writeReady,this,&PTCPSocket::onWrite);
    if(server==nullptr){
        m_server=new PTCPServer(10,this);
    }
    m_server=server;
}

std::string PTCPSocket::getIp(){
   // inet_aton(ip.c_str(),&serverAddr.sin_addr);
   return std::string(inet_ntoa(Socketaddr.sin_addr));
}

int PTCPSocket::getPort(){
    return ntohs(Socketaddr.sin_port);
}

PTCPSocket::~PTCPSocket(){
    if(scfd!=0){
        ::close(scfd);
        scfd=0;
    }
}

 void PTCPSocket::send(const char * data,int size){
    std::unique_lock<std::mutex> locker(cache_lock);
    sendCaceh.push(data,size);
    m_server->eableEvent(scfd,EVFILT_WRITE,&socketEvent);
    writeReady();
 }

 void PTCPSocket::onWrite(){
    std::unique_lock<std::mutex> locker(cache_lock);
    char buffer[4096]={0};
    int ret=0;
    while(sendCaceh.size()>0){
       ret=sendCaceh.pop(buffer,4096);
       ::send(scfd,buffer,ret ,0);
    }
 }

int PTCPSocket::read(char * data,int size){
    int ret= ::read(scfd,data,size);
    if(ret==0){
        m_server->deleteEvent(scfd,EVFILT_READ,&socketEvent);
        ::close(scfd);
        scfd=0;
    }
    return ret;
}

PTCPServer::PTCPServer(int maxCheckEventSize,PObject * parent):PThread(parent){
    kqueue_handle=kqueue();
    checkeventList=new struct kevent[maxCheckEventSize];
    MaxEventCheckSize=maxCheckEventSize;
}

PTCPServer::~PTCPServer(){
    if(listen_fd!=0){
        ::close(listen_fd);
    }
    listen_fd=0;
}


bool PTCPServer::bind(const std::string & ip,int port){
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

bool PTCPServer::listen(int MaxListenSize){
    int ret= ::listen(listen_fd, MaxListenSize);
    if(ret< 0){
        errorL("listen:" << strerror(errno));
        return false;
    }
    addEvent(listen_fd, EVFILT_READ|EVFILT_WRITE,&listen_event);
    
    return true;
}

void PTCPServer::run(){
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
                PTCPSocket * Psocket=static_cast<PTCPSocket*>(socketPtr);
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

void PTCPServer::exit(){
    PEventLoop::exit();
    deleteEvent(listen_fd,EVFILT_READ|EVFILT_WRITE,&listen_event);
    
}

void PTCPServer::accept(struct kevent * ke){
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
        PTCPSocket * client_socket=new PTCPSocket(client_fd,this,this);
        client_socket->Socketaddr=clientAddr;
        addEvent(client_fd,EVFILT_READ|EVFILT_WRITE,&client_socket->socketEvent,(void *)client_socket);
        connected(client_socket);
    }
   

}


void PTCPServer::addEvent(int fd,int event, struct kevent *e,void *dataPtr){
    int ret = 0;
   
    EV_SET(e, fd, event, EV_ADD, 0, 0, dataPtr);
    ret = kevent(kqueue_handle, e, 1, NULL, 0, NULL);
    if (ret < 0){
         errorL("addEvent:" << strerror(errno));
    }
}

void PTCPServer::deleteEvent(int fd,int event, struct kevent *e){
    int ret = 0;
    
    EV_SET(e, fd, event, EV_DELETE, 0, 0, NULL);
    ret = kevent(kqueue_handle, e, 1, NULL, 0, NULL);
   if (ret < 0){
         errorL("deleteEvent:" << strerror(errno));
    }
}

void PTCPServer::eableEvent(int fd,int event, struct kevent *e){
    int ret = 0;
   // debugL("file description:" << fd);
    EV_SET(e, fd, event, EV_ADD|EV_ENABLE, 0, 0, NULL);
    ret = kevent(kqueue_handle, e, 1, NULL, 0, NULL);
    if (ret < 0){
         errorL("eableEvent:" << strerror(errno));
         
    }
    debugL("ret:" << fd);
}

void PTCPServer::disableEvent(int fd,int event, struct kevent *e){

    int ret = 0;
    EV_SET(e, fd, event, EV_DISABLE, 0, 0, NULL);
    ret = kevent(kqueue_handle, e, 1, NULL, 0, NULL);
    if (ret < 0){
         errorL("disableEvent:" << strerror(errno));
    }
}



#endif


