#include "PTimer.h"
#ifdef _WINDOWS
std::shared_ptr<void> PTimer::__timeQueue =nullptr;

PTimer::PTimer(PObject* parent) :parent_(parent)
{
	
	if (__timeQueue == nullptr) {
		HANDLE _timeq = CreateTimerQueue();//
		__timeQueue.reset(_timeq, [=](void * p) {
			if (DeleteTimerQueueEx(p, INVALID_HANDLE_VALUE)) {

			}
		});
	}
	
}

void PTimer::start(int second)
{
	startWithMillisecond(second * 1000);
}

void PTimer::startWithMillisecond(long long msecond)
{
	HANDLE hNewTimer_;
	CreateTimerQueueTimer(&hNewTimer_, __timeQueue.get(),
		&PTimer::timeCallback, this, msecond , msecond, true);
	hNewTimer = hNewTimer_;
}

void PTimer::stop()
{
	if (NULL == hNewTimer) return;


	//
	BOOL iRet = DeleteTimerQueueTimer(__timeQueue.get(), hNewTimer, INVALID_HANDLE_VALUE);
	DWORD iErr = GetLastError();
	if (0 == iRet) {
		if (ERROR_IO_PENDING == iErr)
			return;
		perror(" DeleteTimerQueueTimer ");
		return ;
	}
}

VOID PTimer::timeCallback(PVOID parg, BOOLEAN TimerOrWaitFired)
{
	PTimer* timer = (PTimer*)parg;
	timer->timeout();

}
#else

#endif