
#ifndef _P_TIMER
#define _P_TIMER


#include "PObject.h"
#ifdef _WINDOWS 
#include <Windows.h>

class PTimer :public PObject {
public:
	PTimer(PObject* parent=nullptr) ;

	void start(int second);

	void startWithMillisecond(long long msecond);

	void stop();

	SIGNAL_(:)
		_SIGNAL_VOID(PTimer, timeout)



private:
	HANDLE hNewTimer=nullptr;
	static std::shared_ptr<void> __timeQueue;//
	static VOID CALLBACK timeCallback(PVOID parg, BOOLEAN TimerOrWaitFired);
	PObject* parent_ = nullptr;
};






#else


#endif


#endif // !_P_TIMER