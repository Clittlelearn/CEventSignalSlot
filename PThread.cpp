#include "PThread.h"



PThread::PThread(PObject* parent):m_parent(parent)
{
	connect(this, &PThread::started, this, &PThread::run);
}
void PThread::run() {

}

void PThread::start()
{
	_th = std::thread([=] {
		moveToThisThread();
			PCore p;
			this->started();
			p.exec();
		});

	_th.detach();
	//_th.join();

}

void PThread::moveToThisThread() {
	setObjectThread();
}