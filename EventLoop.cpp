#include "EventLoop.h"
#include <thread>
#include "debug.h"

std::map<std::thread::id, PEventLoop::CoreData::ptr> PEventLoop::object_map;
std::mutex PEventLoop::theadIdMutex;

PEventLoop::PEventLoop()
{
	setObjectThread();
}

void PEventLoop::exec()
{
	if (object_loop == nullptr)
	{
		errorL("object_loop is nullptr") return;
	}
	PEvent *e = nullptr;
	while (!object_loop->toExit)
	{
		{
			std::unique_lock<std::mutex> lock(object_loop->LoopMutex);
			if (object_loop->LoopEvent.size() == 0)
			{

				object_loop->cond_t.wait(lock);
			}
			e = object_loop->LoopEvent.top();
			object_loop->LoopEvent.pop();
		}
		e->Invke();
	}
}

void PEventLoop::exit()
{
	if (object_loop == nullptr)
	{
		errorL("object_loop is nullptr") return;
	}
	::exit(0);
}

void PEventLoop::waitQuit()
{
	object_loop->toExit = true;
	object_loop->cond_t.notify_all();
}

void PEventLoop::setSlotOf(slot_base *e)
{
	e->setSlotOf(this);
}

void PEventLoop::addEvent(PEvent *e)
{
	if (object_loop == nullptr)
	{
		errorL("object_loop is nullptr") return;
	}

	std::unique_lock<std::mutex> lock(object_loop->LoopMutex);
	object_loop->LoopEvent.push(e);
	lock.unlock();
	object_loop->cond_t.notify_all();

}


void PEventLoop::setEventPriority(slot_base *e, int Priority)
{
	e->setPriority(Priority);
}

void PEventLoop::setObjectThread()
{
	std::unique_lock<std::mutex> lock(theadIdMutex);
	std::thread::id id_ = std::this_thread::get_id();
	if (object_loop == nullptr)
	{
		// find loop;
		auto target_this_loop = object_map.find(id_);
		if (target_this_loop != object_map.end())
		{
			// finded !!
			object_loop = target_this_loop->second;
		}
		else
		{
			// not found
			object_loop = std::make_shared<PEventLoop::CoreData>();
			object_map[id_] = object_loop;
		}
	}
	else
	{

		auto target_this_loop = object_map.find(id_);
		if (target_this_loop != object_map.end())
		{
			errorL("The object found but not set!");
		}
		else
		{
			object_loop = std::make_shared<PEventLoop::CoreData>();
			object_map[id_] = object_loop;
		}
	}
}

void slot_base::setSlotOf(PEventLoop *loop)
{
	loopPtr = loop;
}

void slot_base::addSelftToWorkThread()
{
	if (loopPtr)
	{
		loopPtr->addEvent(this);
	}
	else
	{
		errorL("target_loop is nullptr!!");
	}
}

void slot_base::setPriority(int Priority_)
{
	this->Priority = Priority_;
}
