
#include <utility>
#include <memory>
#include <vector>
#include <iostream>
#include <functional>
#include <vector>
#include "Cycliclist.hpp"
#include <list>
#include "MessageQueue.h"
#include <queue>
#include "PObject.h"
#include "PThread.h"


void test_print(std::string msg) {
	std::cout << "msg:" << msg << std::endl;
}

int test_print_int_int(int a,int b) {
	std::cout << "a + b = " << a + b << std::endl;
	return 0;
}

void test_print_char(char c) {
	std::cout << "c: = " << c<< std::endl;
}
void test_print_int(int a){
	std::cout << "a: = " << a << std::endl;
}

struct testDemo {
	testDemo(const std::string name) {
		this->name = name;
	}
	testDemo(const testDemo& dm) {
		std::cout << "copy demo:" << name << std::endl;
	}

	void to_print(std::string msg) {
		std::cout << "msg:" << name << std::endl;
	}
	int operator()(int a, int b) {
		std::cout << "a + b = " << a + b << std::endl;
		return a+b;
	}
	std::string name;
};

int test_arg(testDemo d) {
	std::cout << "sizeof:" << d(23,78) << std::endl;
	return 0;
}


class Widget:public PObject{
public:

	

	Widget() {
		connect(this, &Widget::int_signal, this, &Widget::int_slot);

		connect(this, &Widget::hello, [](std::string name, int age, bool sex) {
			std::cout << "name:" << name << std::endl;
			std::cout << "age:" << age << std::endl;
			std::cout << "sex" << sex << std::endl;
		});
		
	}

	
	
	_SIGNAL(Widget,hello,
	_ARGS(const std::string & name,int age,bool sex),
	_ARGS(const std::string &,int,bool),
	_ARGS(name,age,sex)
	)

	/*void hello(const std::string & name,int age,bool sex) {
		emit<Widget,const std::string&,int,bool>(this, &Widget::hello, name, age, sex);
	}*/

	_SIGNAL(Widget,int_signal,
	_ARGS(int a),
	_ARGS(int),
	_ARGS(a)
	)
	/*void int_signal(int a) {
		emit(this, &Widget::int_signal, a);
	}*/


		
	
	

	void ttl(const std::string& msg, int b) {
		std::cout << msg << "value:" << b << std::endl;
	}

	//slot

	void world(const std::string& msg,int b) {
		std::cout << msg << "value:"  << b<< std::endl;
	}

	void int_slot(int bbl) {
		static int i = 0;
		std::cout << "bbl:" << bbl << std::endl;
		if (i < 10000) {
			//int_signal(i++);
		}
	}
	
	void print_str(int str) {
		std::cout << "str:" << str << std::endl;
	}
};



class Button :public PObject {
public:
	Button(){
		connect(this, &Button::void_test, this, &Button::void_slot);

	}
	
	_SIGNAL(Button, clicked,
		_ARGS(int pos),
		_ARGS(int),
		_ARGS(pos))

	_SIGNAL_VOID(Button,void_test)

	/*void clicked(const std::string& pos) {
		
		emit<Button,const std::string&>(this, &Button::clicked, pos);
	}*/

	/*void void_test() {
		emit(this, &Button::void_test);
	}*/

	void void_slot() {
		std::cout << "hhhh" << std::endl;
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
	{
		PCore core;
		Widget* widget = new Widget;

		ThreadTest thread_t;


		thread_t.start();

		//thread_t.start();

		widget->int_signal(45);

		widget->hello("wangbl", 78, false);





		std::cout << "hello" << std::endl;

		Button bt;

		widget->connect(&bt, &Button::clicked, widget, &Widget::print_str);

		bt.void_test();



		bt.clicked(13);

		core.exec();
		
	}
	






	return 0;
}




