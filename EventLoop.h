#ifndef __LOOP_M_
#define __LOOP_M_
#include <map>
#include <queue>
#include <functional>
#include <utility>
#include <mutex>
#include <condition_variable>
#include <tuple>
#include <map>
#include <thread>

#include "debug.h"
#include "CAS.h"


class signal_base {
public:
	signal_base() {}
	virtual ~signal_base() {}
	virtual void deleteSlot(uint64_t id) {}
	virtual bool isEmpty() { return false; }
	CAS spinLock;

};

class PEventLoop;
struct Event_compare;

class PEvent {
public:

	enum {
		CUSTM_FUNC = 0,
		TIMER_FUNC = 1
	};


	virtual void Invke() {}
protected:
	friend PEventLoop;
	friend Event_compare;
	std::thread::id thread_id;
	PEventLoop* loopPtr=nullptr;
	int Priority = 0;
};

struct Event_compare {
	bool operator()(PEvent*& L, PEvent*& R) {
		return L->Priority < R->Priority;
	}
};

class slot_base :protected PEvent {
public:
	void setSlotOf(PEventLoop* loop);
	void addSelftToWorkThread();
	void setPriority(int Priority_);
	uint64_t slot_id;
	
};


class PEventLoop {
public:
	PEventLoop();
	void exec();
	void exit();
	void waitQuit();

	

protected:
	friend slot_base;
	void addEvent(PEvent* e);


	void setEventPriority(slot_base*e,int Priority);
	void setSlotOf(slot_base* e);
	void setObjectThread();
	

	struct CoreData {
		using ptr= std::shared_ptr<CoreData>;
		std::atomic<bool> toExit;//队列中是否有事件
		std::mutex LoopMutex;
		std::condition_variable cond_t;
		//std::queue<PEvent*> LoopEvent;
		std::priority_queue<PEvent*, std::vector<PEvent*>, Event_compare> LoopEvent;

		std::thread::id work_id;
	};

	CoreData::ptr object_loop=nullptr;
	static std::mutex theadIdMutex;
	static std::map<std::thread::id, CoreData::ptr> object_map;
	 
};


#endif