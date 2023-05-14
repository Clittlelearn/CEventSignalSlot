#include "BLTimer.h"
#ifdef _WINDOWS
namespace BL {

	std::shared_ptr<void> Timer::__timeQueue = nullptr;

	Timer::Timer(Object* parent) :parent_(parent)
	{

		if (__timeQueue == nullptr) {
			HANDLE _timeq = CreateTimerQueue();//
			__timeQueue.reset(_timeq, [=](void* p) {
				if (DeleteTimerQueueEx(p, INVALID_HANDLE_VALUE)) {

				}
				});
		}

	}

	void Timer::start(int second)
	{
		startWithMillisecond(second * 1000);
	}

	void Timer::startWithMillisecond(long long msecond)
	{
		HANDLE hNewTimer_;
		CreateTimerQueueTimer(&hNewTimer_, __timeQueue.get(),
			&Timer::timeCallback, this, msecond, msecond, true);
		hNewTimer = hNewTimer_;
	}

	void Timer::stop()
	{
		if (NULL == hNewTimer) return;


		//
		BOOL iRet = DeleteTimerQueueTimer(__timeQueue.get(), hNewTimer, INVALID_HANDLE_VALUE);
		DWORD iErr = GetLastError();
		if (0 == iRet) {
			if (ERROR_IO_PENDING == iErr)
				return;
			perror(" DeleteTimerQueueTimer ");
			return;
		}
	}

	VOID Timer::timeCallback(PVOID parg, BOOLEAN TimerOrWaitFired)
	{
		Timer* timer = (Timer*)parg;
		timer->timeout.emit();

	}

}
#else

#endif