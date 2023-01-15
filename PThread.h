#ifndef _P_THREAD_
#define _P_THREAD_

#include <thread>
#include "PObject.h"


class PThread:public PObject{
public:

	PThread(PObject* parent);

	void start();

	virtual void run();

SIGNAL_(:)
	_SIGNAL_VOID(PThread,started)

protected:
	void moveToThisThread();

private:
	std::thread _th;
	PObject* m_parent;
};


#endif // !_P_THREAD_