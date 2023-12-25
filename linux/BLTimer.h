
#ifndef _BLTIMER_H
#define _BLTIMER_H
#include <bits/types/sigevent_t.h>
#include <cstdint>
#include <functional>
#include <time.h>
#include <sys/time.h>

namespace BL {

class LinuxTimer{

    public:
    LinuxTimer();
    ~LinuxTimer();

    void StartWithSecond(int second,const std::function<void()> & func);

    void StartWithNSecond(int nsced);

    void Stop();

    

private:

    friend void timerHandler(union sigval v);

    void inittime();
    struct sigevent evp;
    struct itimerspec ts;
    timer_t timerId;
    std::function<void()> callback;
};


}


#endif