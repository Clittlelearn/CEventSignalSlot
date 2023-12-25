#include "BLTimer.h"
#include "debug.h"
#include <cerrno>
#include <cstring>
#include <ctime>
#include <signal.h>
LOGINSTANCE("linuxTime.log")
namespace BL {


void timerHandler(union sigval v){
    LinuxTimer * t=(LinuxTimer *)v.sival_ptr;
    t->callback();
}

LinuxTimer::LinuxTimer(){
    int ret;
    memset(&evp, 0, sizeof(evp));
    evp.sigev_value.sival_ptr = this;
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = timerHandler;
    ret = timer_create(CLOCK_REALTIME, &evp, &timerId);
    if(ret<0){
        errorL(strerror(errno));
    }
}

void LinuxTimer::StartWithSecond(int second,const std::function<void()> & func){
    int ret=0;
    struct timespec timeNow;
    clock_gettime(CLOCK_REALTIME, &timeNow);

    timeNow.tv_sec+=second;
    ts.it_interval.tv_sec = second;
    ts.it_interval.tv_nsec = 0;
    ts.it_value=timeNow;
    ret = timer_settime(timerId, TIMER_ABSTIME, &ts, NULL);
    callback=func;
    if(ret<0){
        errorL(strerror(errno));
    }
    
}

void LinuxTimer::Stop(){
    int ret=0;
     ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = 0;
    ts.it_value.tv_nsec=0;
    ts.it_value.tv_sec=0;
    ret = timer_settime(timerId, TIMER_ABSTIME, &ts, NULL);
    if(ret<0){
        errorL(strerror(errno));
    }
}

LinuxTimer::~LinuxTimer(){
    Stop();
    timer_delete(timerId);
}


}