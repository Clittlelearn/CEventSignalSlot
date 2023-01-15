
#include <utility>
#include <memory>
#include <vector>
#include <iostream>
#include <functional>
#include <vector>
#include <list>
#include "MessageQueue.h"
#include <queue>
#include "PObject.h"
#include "PThread.h"
#include "PTimer.h"




class Widget :public PObject {
public:

	Widget() {
		connect(this, &Widget::hello, this, &Widget::test_slot_A);
		connect(this, &Widget::hello, this, &Widget::test_slot_B);

		connect(this, &Widget::hello, 0x33, [] {
			infoL("This is lamda ");
			});
	}

SIGNAL_(:)
	_SIGNAL_VOID(Widget,hello)



	void test_slot_A() {
		infoL("I am A");
	}
	
	void test_slot_B() {
		infoL("I am B");
	}



};


class ThreadTest :public PThread {
public:
	ThreadTest(PObject * parent=nullptr):PThread(parent){}

	void run() {
		for (int i = 0;i < 30; i++) {
			infoL("thread test:" << std::this_thread::get_id());
			::_sleep(2000);
		}
		
	}
};

//windows timer test






int main() {
	
	PCore core;
	Widget widget;

	PTimer timer;

	timer.connect(&timer, &PTimer::timeout, 0x45, [=] {
		infoL("I am Timer:");
		});


	timer.start(2);

	widget.disconnect(&widget, &Widget::hello, &widget, &Widget::test_slot_B);
	widget.disconnect(&widget, &Widget::hello, 0x33);
	widget.hello();


	core.exec();

	return 0;
}




