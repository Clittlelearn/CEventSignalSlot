#include "BLEventLoop.h"
#include "debug.h"

namespace BL {
std::map<std::thread::id, EventLoop::CoreData::ptr> EventLoop::object_map;
std::mutex EventLoop::theadIdMutex;

EventLoop::EventLoop() {
}

void EventLoop::exec() {
    theadIdMutex.lock();
    if (object_loop == nullptr) {
        errorL("object_loop is nullptr");
        theadIdMutex.unlock();
        return;
    }
    CoreData::ptr coreLoop = object_loop;
    theadIdMutex.unlock();
    Event *e = nullptr;

    while (!coreLoop->toExit) {
        {
            std::unique_lock<std::mutex> lock(coreLoop->LoopMutex);
            if (coreLoop->LooEvent.size() == 0) {

                coreLoop->cond_t.wait(lock);
            }
            e = coreLoop->LooEvent.top();
            coreLoop->LooEvent.pop();
        }
        e->Invke();
    }
}

void EventLoop::exit() {
    if (object_loop == nullptr) {
        errorL("object_loop is nullptr") return;
    }
    ::exit(0);
}

void EventLoop::waitQuit() {
    object_loop->toExit = true;
    object_loop->cond_t.notify_all();
}

std::thread::id EventLoop::getWorkThreadLoopId() {
    if (object_loop != nullptr) {
        return object_loop->work_id;
    }
    return std::thread::id();
}

void EventLoop::addEvent(Event *e) {
    if (object_loop == nullptr) {
        errorL("object_loop is nullptr plase call moveToSelfThread !") return;
    }

    std::unique_lock<std::mutex> lock(object_loop->LoopMutex);
    object_loop->LooEvent.push(e);
    lock.unlock();
    object_loop->cond_t.notify_all();
}

void EventLoop::setObjectThread(const std::thread::id &id) {
    std::unique_lock<std::mutex> lock(theadIdMutex);
    std::thread::id id_ = id;
    if (object_loop == nullptr) {
        // find loop;
        auto target_this_loop = object_map.find(id_);
        if (target_this_loop != object_map.end()) {
            // finded !!
            object_loop = target_this_loop->second;
        } else {
            // not found
            object_loop = std::make_shared<EventLoop::CoreData>(id);
            object_map[id_] = object_loop;
        }
    } else {

        auto target_this_loop = object_map.find(id_);
        if (target_this_loop != object_map.end()) {
            object_loop = target_this_loop->second;
        } else {
            object_loop = std::make_shared<EventLoop::CoreData>(id);
            object_map[id_] = object_loop;
        }
    }
}

void Event::setPriority(int Priority_) { this->Priority = Priority_; }

} // namespace BL
