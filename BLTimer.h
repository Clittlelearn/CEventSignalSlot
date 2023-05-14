
#ifndef _P_TIMER
#define _P_TIMER

#include "BLObject.h"
#ifdef _WINDOWS 
#include <Windows.h>

namespace BL {


	class Timer :public Object {
	public:
		Timer(Object* parent = nullptr);

		void start(int second);

		void startWithMillisecond(long long msecond);

		void stop();


		signal_<> timeout;



	private:
		HANDLE hNewTimer = nullptr;
		static std::shared_ptr<void> __timeQueue;//
		static VOID CALLBACK timeCallback(PVOID parg, BOOLEAN TimerOrWaitFired);
		Object* parent_ = nullptr;
	};




}


#endif


#endif // !_P_TIMER