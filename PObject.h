#ifndef _POBJECT_M_
#define _POBJECT_M_
#include<map>
#include<functional>
#include<vector>
#include <string>
#include <iostream>
#include <tuple>
#include "EventLoop.h"
#include "debug.h"

#if _MSC_VER
#define PlaceHolder std::_Ph
#else
#define PlaceHolder std::_Placeholder
#endif

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

template<typename _ST, typename ..._Ts>
std::pair<std::string, uint64_t> getId(_ST* _OBJ_S, void(_ST::* _SIG)(_Ts...)) {
	long long object_s = (long long)_OBJ_S;
	long long funtion_s = (long long)supercast<void(*)(_Ts...)>(_SIG);
	/*std::string id = std::to_string(object_s);
	id += std::to_string(funtion_s);*/

	return { std::to_string(object_s) +=std::to_string(funtion_s) ,funtion_s };
}



class PObject;

template<typename F, typename... Ts>
class slot :public slot_base {
public:
	slot(F&& f, uint64_t s_id) :f_(std::forward<F>(f)) {
		slot_id = s_id;
	}
	
	void setArgPtr(std::tuple<Ts...> * args_) {
		args = args_;
	}


	void Invke()override {
		std::apply(f_, *args);
	}
protected:
	F f_;
	std::tuple<Ts...>* args;
};

template<typename F, typename... _Ts>
class signal :public signal_base
{
public:
	signal() { }
	void setArgs(const std::tuple<_Ts...>&& args_t) {
		_args = args_t;
	}
	~signal() {
		for (auto e : all_slot) {
			delete e;
		}
	}

	void addSlot(slot<F, _Ts...>* signal_t) {
		all_slot.push_back(signal_t);
	}

	bool isEmpty()override {
		return all_slot.empty();
	}

	void deleteSlot(uint64_t id) override{
		for (auto e = all_slot.begin(); e != all_slot.end();) {
			if ((*e)->slot_id == id) {
				slot_base*sl = (*e);
				e=all_slot.erase(e);
				delete sl;
			}
		}
	}

	void emit() {
		for (auto e : all_slot) {
			static_cast<slot<F, _Ts...>*>( e )->setArgPtr(&_args);
			e->addSelftToWorkThread();
		}
	}
private:
	std::vector<slot_base*> all_slot;
	std::tuple<_Ts...> _args;

};



class PObject:protected PEventLoop{
public:
	PObject(PObject* parent = nullptr) :m_parent(parent) {}

	
	virtual ~PObject();

	virtual void setPriority(slot_base * s) {
		setEventPriority(s, (int)PEvent::CUSTM_FUNC);
	}


	template<typename _ST, typename _RT, typename ..._Ts>
	void disconnect(_ST* _OBJ_S, void(_ST::* _SIG)(_Ts...), _RT* __OBJ_R, void(_RT::* _SLOT)(_Ts...)) {
		std::pair<std::string, uint64_t> id = getId(_OBJ_S, _SIG);
		uninstallSlot(id);
		
	}

	template<typename _ST, typename F, typename ...Ts>
	void disconnect(_ST* _OBJ_S, void(_ST::* _SIG)(Ts...)) {
		std::pair<std::string, uint64_t> id = getId(_OBJ_S, _SIG);
		uninstallSlot(id);
	}

	template<typename _ST, typename _RT, typename ..._Ts>
	 void connect(_ST* _OBJ_S,void(_ST::* _SIG)(_Ts...),_RT* __OBJ_R,void(_RT::* _SLOT)(_Ts...))
	 {
		 std::pair<std::string, uint64_t> id = getId(_OBJ_S, _SIG);

		slot < std::function<void(_Ts...)>, std::decay_t<_Ts>...>* slot_
		= __OBJ_R->createSlot < std::function<void(_Ts...)>, std::decay_t<_Ts>...>(
			std::forward< std::function<void(_Ts...)>>(Bind(_SLOT, __OBJ_R)),
			id.second
			);

		private_connect<std::function<void(_Ts...)>,std::decay_t<_Ts>...>(id.first, slot_);
	}

	template<typename _ST, typename F, typename ...Ts>
	void connect(_ST* _OBJ_S,void(_ST::* _SIG)(Ts...),F &&f) 
	{
		std::pair<std::string, uint64_t> id = getId(_OBJ_S, _SIG);
		
		
		slot<std::function<void(Ts...)>, std::decay_t<Ts>...> * slot_ =
			_OBJ_S->createSlot<std::function<void(Ts...)>, std::decay_t<Ts>...>(std::forward<F>(f),id.second);

		private_connect<std::function<void(Ts...)>,std::decay_t<Ts>...>(id.first,slot_);
	}

	template<typename F, typename... _Ts>
	slot<F, std::decay_t<_Ts>...>* createSlot(F&& f,uint64_t sid) {
		slot<F, std::decay_t<_Ts>...>* n_slot = new slot<F, std::decay_t<_Ts>...>(std::forward<F>(f),sid);
		setSlotOf(n_slot);
		setPriority(n_slot);
		return n_slot;
	}
protected:



	signal_base* getSignalObject(const std::string& _SIG);

	void addSignalObject(const std::string& _SIG, signal_base* obj);

	template<typename F, typename ..._Ts>
	void private_connect(const std::string& _SIG, slot < F, std::decay_t<_Ts>...>* slot_t) {
		auto obj = getSignalObject(_SIG);

		if (obj != nullptr) {
			signal<F, std::decay_t<_Ts>...>* signal_t = static_cast<signal<F, std::decay_t<_Ts>...>*>(obj);
			signal_t->addSlot(slot_t);

		}
		else {

			signal<F, std::decay_t<_Ts>...>* signal_t = new signal< F, std::decay_t<_Ts>...>;
			signal_t->addSlot(slot_t);
			addSignalObject(_SIG, signal_t);
		}
	}


	template <typename _ST, typename ..._Ts>
	void emit(_ST* object, void(_ST::* _SIG)(_Ts...), _Ts... args) {
		std::pair<std::string, uint64_t> id = getId(object, _SIG);
		auto sobj = getSignalObject(id.first);
		if (sobj != nullptr)
		{
			signal< std::function<void(_Ts...)>,std::decay_t<_Ts>...>* signal_t =
				static_cast<signal< std::function<void(_Ts...)>,std::decay_t<_Ts>...> *>(sobj);
			signal_t->setArgs(std::tuple<std::decay_t<_Ts>...>(std::forward<_Ts>(args)...));
			signal_t->emit();
		}
	}

private:

	void uninstallSlot(std::pair< std::string, uint64_t>& pa);
	std::map<std::string, signal_base*> __ALL_SIGNALS_;
	PObject* m_parent;

};

class PCore :public PEventLoop 
{
public:
	PCore() = default;
};



#define SIGNAL_(a)

#define _ED(a) 


#define _ARGS(...)\
	 _ED(0) ##__VA_ARGS__ 

#define _SIGNAL(className,signalName,one,tow,three)\
	void signalName(one){\
		emit<className,tow>(this,&className::signalName,three);\
	}

#define _SIGNAL_VOID(className,signalName)\
	void signalName(){\
		emit<className>(this,&className::signalName);\
	}


#endif // !_POBJECT_M_
