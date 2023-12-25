#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <iostream>
#include <memory>
#include <mutex>

template<typename T>
class Singleton {
public:
    template<typename ...Ts>
	static T* GetInstance(const Ts&... ts)
	{
		static std::once_flag s_flag;
		std::call_once(s_flag, [&]() {
			m_pSington.reset(new T(ts...));
		});

		return m_pSington.get();
	}

protected:
	Singleton(){};
	~Singleton(){};

	static std::shared_ptr<T> m_pSington;

private:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

private:
	
};

template<typename T> std::shared_ptr<T> Singleton<T>::m_pSington;


#endif   //__SINGLETON_H__
