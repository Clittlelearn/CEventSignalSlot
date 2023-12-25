#ifndef __LOOP_M_
#define __LOOP_M_
#include "CAS.h"
#include "debug.h"
#include <condition_variable>
#include <functional>
#include <map>
#include <queue>
#include <thread>
#include <tuple>
#include <utility>

namespace BL {
class EventLoop;
struct EventCompare;

class Event {
  public:
    enum { CUSTM_FUNC = 0, TIMER_FUNC = 1 };

   // virtual void Invke() {}
    std::function<void()> InvkeCallback=nullptr;

    void setPriority(int Priority_);
    uint64_t slot_id;
    CAS spinLock;

  protected:
    friend EventLoop;
    friend EventCompare;
    std::thread::id thread_id;
  //  EventLoop *loopPtr = nullptr;
    int Priority = 0;
};

struct EventCompare {
    bool operator()(Event *&L, Event *&R) {
        return L->Priority < R->Priority;
    }
};


class EventLoop {
  public:
    EventLoop();
    void exec();
    void exit();
    void waitQuit();
    std::thread::id getWorkThreadLoopId();

    void addEvent(Event *e);

  protected:
    friend Event;

    void setObjectThread(const std::thread::id &id);

    struct CoreData {
        CoreData(std::thread::id id) { work_id = id; toExit =false;}
        using ptr = std::shared_ptr<CoreData>;
        std::atomic<bool> toExit ; // 队列中是否有事件
        std::mutex LoopMutex;
        std::condition_variable cond_t;
        // std::queue<Event*> LooEvent;
        std::priority_queue<Event *, std::vector<Event *>, EventCompare>
            LooEvent;

        std::thread::id work_id;
    };

    CoreData::ptr object_loop = nullptr;
    static std::mutex theadIdMutex;
    static std::map<std::thread::id, CoreData::ptr> object_map;
};
} // namespace BL

#endif