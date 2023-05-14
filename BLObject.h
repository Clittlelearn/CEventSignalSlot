#ifndef _Object_M_
#define _Object_M_
#include<map>
#include<functional>
#include<vector>
#include <string>
#include <iostream>
#include <tuple>
#include "BLEventLoop.h"
#include "debug.h"

#if _MSC_VER
#define PlaceHolder std::_Ph
#elif __MACH__
#define PlaceHolder std::placeholders::__ph
#elif __linux__
#define PlaceHolder std::_Placeholder
#elif __GNUC__
#define PlaceHolder std::_Placeholder
#endif

namespace BL {

template<int N, int...I>
struct MakeSeqs : MakeSeqs<N - 1, N - 1, I...> {}; 

template<int...I>
struct MakeSeqs<1, I...>
{
	template<typename T, typename R, typename...Args>
	static auto bind(T* obj, R(T::* _Func)(Args...)) -> decltype(std::bind(_Func, obj, PlaceHolder<I>{}...))
	{
		return std::bind(_Func, obj, PlaceHolder<I>{}...);
	}
};

template <typename T, typename R, typename...Args>
auto Bind(T* t, R(T::* f)(Args...)) -> decltype(MakeSeqs<sizeof...(Args) + 1>::bind(t, f))
{
	return MakeSeqs<sizeof...(Args) + 1>::bind(t, f);
}

template <typename T, typename R, typename...Args>
auto Bind(R(T::* f)(Args...), T* t) -> decltype(MakeSeqs<sizeof...(Args) + 1>::bind(t, f))
{
	return MakeSeqs<sizeof...(Args) + 1>::bind(t, f);
}

template <typename T, typename F>
T supercast(F f)
{
	union FT
	{
		T   t;
		F   f;
	};
	FT ft;
	ft.f = f;
	return ft.t;  //超级转换
}




class Object;

template<typename F, typename... Ts>
class Slot :public Event {
public:
	Slot(F&& f, uint64_t s_id) :f_(std::forward<F>(f)) {
		slot_id = s_id;
	}
	
	void addArg(const std::tuple<Ts...> & args_) {
		args.push(args_);
	}



	void Invke()override {
		spinLock.lock();
		auto arg=args.front();
		args.pop();
		spinLock.unlock();
		std::apply(f_, arg);
	}
protected:
	F f_;
	std::queue<std::tuple<Ts...>> args;
};


class Object:public EventLoop{
public:
	Object(Object* parent = nullptr)  {
        std::thread::id id = std::this_thread::get_id();
	    setObjectThread(id);
    }
    void addToEventLoop(Event * s){
         s->setPriority((int)Event::CUSTM_FUNC);
         addEvent(s);
    }

private:

};



template<typename ...ARGS>
class signal_{
    public:
        signal_() = default;
        ~signal_() {
            for (auto p : slots) {
                delete p.second.second;
            }

            for (auto p : lamda_slots) {
                delete p.second;
            }
        }
    template<typename _RT>
	void connect(Object * threadof,_RT *_OBJ_R,void(_RT::* _SLOT)(ARGS...)){
          Event * sobj=new Slot<std::function<void(ARGS...)> , std::decay_t<ARGS>...>(Bind(_SLOT,_OBJ_R),0);
          long long fid=(long long)supercast<void(*)(ARGS...)>(_SLOT);
          slots[fid]={(Object*)threadof,sobj};
    }

    void connect(Object * ssobj,const std::function<void(ARGS...)> &f){
        Event * sobj=new Slot<std::function<void(ARGS...)> , std::decay_t<ARGS>...>(std::function<void(ARGS...)>(f),0);
        lamda_slots.push_back({(Object*)ssobj,sobj});
    }

    void emit(ARGS ... args_t){
        Slot<std::function<void(ARGS...)>, std::decay_t<ARGS>...> * slot=nullptr;
        for(auto s:slots){
            slot=static_cast< Slot<std::function<void(ARGS...)>, std::decay_t<ARGS>...> *>(s.second.second);
            slot->addArg(std::tuple<std::decay_t<ARGS>...>(args_t...));
            Object * loop=s.second.first;
            loop->addToEventLoop(s.second.second);
        }

        for(auto s:lamda_slots){
             slot=static_cast< Slot<std::function<void(ARGS...)>, std::decay_t<ARGS>...> *>(s.second);
            slot->addArg(std::tuple<std::decay_t<ARGS>...>(args_t...));
            Object * loop=s.first;
            loop->addToEventLoop(s.second);
        }
    }

   template<typename _RT>
   void disconnect(_RT* _OBJ_S,void(_RT::* _SLOT)(ARGS...)){
        long long fid=supercast<void(*)(ARGS...)>(_SLOT);
        auto iter=slots.find(fid);
        if(iter!=slots.end()){
            delete iter->second.second;
        }
        slots.erase(fid);
   }
 private:
    std::map<long long ,std::pair<Object*,Event*>> slots;
    std::vector<std::pair<Object*,Event*>> lamda_slots;
};



class PCore :public Object 
{
public:
	PCore() = default;
};


}

#define exegesis(arg)

#endif // !_Object_M_
