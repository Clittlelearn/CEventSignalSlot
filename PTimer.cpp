#include "PTimer.h"

std::shared_ptr<void> PTimer::__timeQueue =nullptr;

PTimer::PTimer(PObject* parent) :parent_(parent)
{
	
	if (__timeQueue == nullptr) {
		HANDLE _timeq = CreateTimerQueue();//创建一个定时器队列
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


	//标记删除, 如果正在执行,error 为INVALID_HANDLE_VALUE , 执行完成后返回
	BOOL iRet = DeleteTimerQueueTimer(__timeQueue.get(), hNewTimer, INVALID_HANDLE_VALUE);
	DWORD iErr = GetLastError();
	if (0 == iRet) {
		if (ERROR_IO_PENDING == iErr)
			return;
		perror(" DeleteTimerQueueTimer 失败!");
		return ;
	}
}

VOID PTimer::timeCallback(PVOID parg, BOOLEAN TimerOrWaitFired)
{
	PTimer* timer = (PTimer*)parg;
	timer->timeout();

}