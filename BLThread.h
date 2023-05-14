#ifndef _P_THREAD_
#define _P_THREAD_

#include <thread>
#include <condition_variable>
#include "BLObject.h"

namespace BL {


class Thread:public Object{
public:

	Thread(Object* parent=nullptr);

	void start();

	virtual void run();

	void exec() = delete;
    
	signal_<> started;

protected:
	void moveToThisThread();

private:
	std::thread _th;
	Object* m_parent;
	
	
};

}


#endif // !_P_THREAD_