#include "debug.h"
#include "Singleton.h"
#include <fstream>
#include <memory>
#include <mutex>
#include <map>


struct GlogData{
    std::map<std::string ,LogHandler::ptr> allLogInstacne;
    std::string globname;
    LogHandler::ptr globp;
};
static std::mutex lock;
GlogData * gl=nullptr;

LogHandler::ptr GetLogInstance(const std::string & name){
    std::unique_lock<std::mutex> locker(lock);
    if(gl==nullptr){
        gl=new GlogData;
    }
    auto iter=gl->allLogInstacne.find(name);
    if(iter==gl->allLogInstacne.end()){
        LogHandler::ptr t=LogHandler::ptr(new LogHandler(name));
        gl->allLogInstacne[name]=t;
        return t;
    }
    return gl->allLogInstacne[name];
}


void SetGLogName(const std::string & name){
    std::unique_lock<std::mutex> locker(lock);
    if(gl==nullptr){
        gl=new GlogData;
        gl->globp=LogHandler::ptr(new LogHandler(name));
    }
}

void GLog(const std::string & meg){
    std::unique_lock<std::mutex> locker(lock);
     if(gl==nullptr){
       return;
    }
    gl->globp->Log(meg);
}