
#include "BLThread.h"
#include "debug.h"
LOGINSTANCE("Thread.log");
namespace BL {


Thread::Thread(Object* parent):m_parent(parent)
{
	started.connect(this,this,&Thread::run);
}
void Thread::run() {
	infoL("hello");
}

void Thread::start()
{
	std::atomic<bool> isLock = false;
	std::mutex startMutex;
	std::condition_variable cond;
	_th = std::thread([&] {
			moveToThisThread();
			if (startMutex.try_lock()) {
				isLock = true;
                 debugL("try_lock ok");
				startMutex.unlock();
			}
			else {
				cond.notify_one();
                debugL("try_lock fail");
			}
			cond.notify_all();
			Core p;
			this->started.emit();
			p.exec();
		});
	_th.detach();
	std::unique_lock<std::mutex> locker(startMutex);

	if (isLock == false) {
		cond.wait(locker);
	}
	infoL("started");
}

void Thread::moveToThisThread() {
	std::thread::id id = std::this_thread::get_id();
	setObjectThread(id);
}

}