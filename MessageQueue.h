

#ifndef MESSAGE_QUEUE_P
#define MESSAGE_QUEUE_P
#include <mutex>
#include <utility>
#include <type_traits>
#include <assert.h>
#include "debug.h"


#define value_assert(v,msg)\
	if(!v){\
		errorL(msg)\
	}


template <typename T>
struct Message
{
	T value;
	Message<T> *next=nullptr;
};



template<typename T>
class MQueue {
public:
	MQueue() {}

	void push_back(Message<T>* mmor) {
		assert(mmor);
		if (last == nullptr || head == nullptr) {
			assert(last==head);
			last = mmor;
			head = mmor;
			m_size++;
		}
		else {
			last->next = mmor;
			last = mmor;
			m_size++;
		}

		
	}

	Message<T>* pop_front() {
		if (head == nullptr) { 
			return head;
		}
		Message<T>* ret = head;
		head = head->next;
		m_size--;
		if (head == nullptr) {
			last = nullptr;
		}
		ret->next = nullptr;
		return ret;
	}

	void to_alloc(int num) {
		for (int i = 0; i < num; i++) {
			allocMemory();
		}
	}

	void allocMemory() {

		if (head == nullptr|| last==nullptr) {
			assert(last == head);
			head = new Message<T>();
			last = head;
			m_size++;
		}
		else {
			Message<T>* n_t = new Message<T>();
			assert(last);
			last->next = n_t;
			last = n_t;
			m_size++;
		}
		
	}
	void allocMemory_test() {

		if (head == nullptr || last == nullptr) {
			assert(last == head);
			head = new Message<T>();
			head->value = test_int++;
			last = head;
			m_size++;
		}
		else {
			Message<T>* n_t = new Message<T>();
			n_t->value = test_int++;
			assert(last);
			last->next = n_t;
			last = n_t;
			m_size++;
		}

	}
	/// <summary>
	/// 释放内存
	/// </summary>
	/// <param name="size_">释放数量</param>
	/// <returns>true 全部释放 false 还有空间</returns>
	bool toFree(int size_) {

		for (int i = 0; i < size_; i++) {
			Message<T>* toD = pop_front();
			if (toD == nullptr) {
				return true;
			}
			else {
				delete toD;
			}
		}
		return false;
	}

	int size() {
		return m_size;
	}

private:
	std::atomic<int> m_size = 0;
	int test_int = 0;
	Message<T>* head=nullptr;
	Message<T>* last=nullptr;
};

template <typename T>
class MessageQueue
{

public:
	MessageQueue()
	{
		
	}

	MessageQueue(int capacity)
	{
		//创建内存
		for (int i = 0; i < capacity; i++) {
			m_cacheQueue.allocMemory();
		}
		m_defualt_size = capacity;

	}

	~MessageQueue() {
		m_cacheQueue.toFree(m_cacheQueue.size());
		m_workQueue.toFree(m_workQueue.size());
	}

	void push_back(const T &value)
	{
		std::unique_lock<std::mutex> lock(messageMutex);
		memoryCheck();
		Message<T>* value_ = m_cacheQueue.pop_front();
		value_assert(value_, "m_cacheQueue size:" + _S(m_cacheQueue.size()))
		value_->value = value;
		m_workQueue.push_back(value_);//加入到工作队列
		
	}

	void push_back(const T &&value)
	{
		std::unique_lock<std::mutex> lock(messageMutex);
		memoryCheck();
		Message<T>* value_ = m_cacheQueue.pop_front();
		value_assert(value_, "m_cacheQueue size:" + _S(m_cacheQueue.size()))
		value_->value = std::forward<T>(value);
		m_workQueue.push_back(value_);//加入到工作队列
	}

	T pop_front()
	{
		std::unique_lock<std::mutex> lock(messageMutex);
		memoryCheck();
		Message<T>* value_ = m_workQueue.pop_front();
		value_assert(value_,"m_workQueue size:"+_S(m_workQueue.size()))
		m_cacheQueue.push_back(value_);
		return value_->value;
	}

	int size() {
		return m_workQueue.size();
	}

private:

	//检查规则
	// 当工作占比大于%90时 申请内存
	// 当工作占比 小于%50 时开始释放内存
	// 当工作占比 大于%80时停止释放内存
	void memoryCheck() {
		LOG_NAME(cache);
		LOG_NAME(work);
		LOG_NAME(total);

		cache.log("cache size:" + _S(m_cacheQueue.size()));
		work.log("work size:" + _S(m_workQueue.size()));
		total.log(_S(m_cacheQueue.size() + m_workQueue.size()));

		float all = m_workQueue.size() + (m_cacheQueue.size() + 1);
		float k = (float)m_workQueue.size() / all;//工作数量

		int to_free_size = m_cacheQueue.size() * 0.25;
		int to_alloc_size = m_workQueue.size() * 0.25;

		to_free_size = (to_free_size > 5) ? to_free_size : 5;
		to_alloc_size = (to_alloc_size > 5) ? to_alloc_size : 5;

		
		debugL("The K is:" + _S(k))
		/*
			工作占比 > 大于 90%
			缓存队列为0
		*/
		if (k > 0.9 ||  m_cacheQueue.size()==0) {
			debugL("to malloc" + _S(to_alloc_size))
			//当工作占比大于90%时
			m_cacheQueue.to_alloc(to_alloc_size);
			return; 
		}
		if (k < 0.5 && m_cacheQueue.size()> m_defualt_size && m_cacheQueue.size() > to_free_size) {
			debugL("to Free" + _S(to_free_size))
			m_cacheQueue.toFree(to_free_size);
		}
		
		
	}
	std::mutex messageMutex;
	MQueue<T> m_workQueue;//工作队列
	MQueue<T> m_cacheQueue;//内存缓存队列
	int m_defualt_size=0;
};

#endif // !MESSAGE_QUEUE_P
