
#ifndef _P_TIMER
#define _P_TIMER



#include "PObject.h"

class PTimer :public PObject {
public:
	PTimer(PObject* parent) :parent_(parent) {}

	void start(int second);

	void stop();

	SIGNAL_(:)
		_SIGNAL_VOID(PTimer, timeout)


private:
	PObject* parent_ = nullptr;
};

#endif // !_P_TIMER