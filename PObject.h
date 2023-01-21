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
#define PlaceHolder std::placeholders::__ph
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
//
//template<typename _ST,typename ..._Ts>
//std::pair<std::string, uint64_t> getId(_ST* _OBJ_S, void(_ST::* _SIG)(_Ts...))
//{
//	long long object_s = (long long)_OBJ_S;
//	long long funtion_s = (long long)supercast<void(*)(_Ts...)>(_SIG);
//	
//
//	return { std::to_string(object_s) +=std::to_string(funtion_s) ,funtion_s };
//}
template<typename _ST, typename _RT, typename ..._Ts>
std::pair<std::string, uint64_t>  GetId(_ST* _OBJ_S, void(_ST::* _SIG)(_Ts...), _RT* __OBJ_R=0x00, void(_RT::* _SLOT)(_Ts...) = 0x00) {

	long long objs, signal_,objr,slot_;
	objs= (long long)_OBJ_S;
	signal_=(long long)supercast<void(*)(_Ts...)>(_SIG);
	objr = (long long)__OBJ_R;
	slot_=(long long)supercast<void(*)(_Ts...)>(_SLOT);

	return { std::to_string(objs ^ signal_),objr ^ slot_ };
}

template<typename _ST, typename ..._Ts>
std::pair<std::string, uint64_t>  GetId(_ST* _OBJ_S, void(_ST::* _SIG)(_Ts...),uint64_t id_) {
	long long objs, signal_;
	objs = (long long)_OBJ_S;
	signal_ = (long long)supercast<void(*)(_Ts...)>(_SIG);
	return { std::to_string(objs ^ signal_) ,id_ };
}



class PObject;

template<typename F, typename... Ts>
class PSlot :public slot_base {
public:
	PSlot(F&& f, uint64_t s_id) :f_(std::forward<F>(f)) {
		slot_id = s_id;
	}
	
	void setArgPtr(std::queue<std::tuple<Ts...>> * args_) {
		args = args_;
	}

	void setSignal(signal_base * sBase_){
		sBase=sBase_;
	}


	void Invke()override {
		auto arg=args->front();
		sBase->spinLock.lock();
		args->pop();
		sBase->spinLock.unlock();
		std::apply(f_, arg);
		
	}
protected:
	F f_;
	signal_base * sBase=nullptr;
	std::queue<std::tuple<Ts...>>* args;
};

template<typename F, typename... _Ts>
class PSignal :public signal_base
{
public:
	PSignal() { }
	void setArgs(const std::tuple<_Ts...>&& args_t) {
		spinLock.lock();
		_args.push(args_t);
		spinLock.unlock();
	}
	~PSignal() {
		for (auto e : all_slot) {
			delete e;
		}
	}

	void addSlot(PSlot<F, _Ts...>* slot_t) {
		
		all_slot.push_back(slot_t);
		slot_t->setSignal(this);
	}

	bool isEmpty()override {
		return all_slot.empty();
	}

	void deleteSlot(uint64_t id) override{
		for (auto e = all_slot.begin(); e != all_slot.end(); ) {
			if ((*e)->slot_id == id) {
				slot_base*sl = (*e);
				e=all_slot.erase(e);
				delete sl;
				return;
			}
			else {
				e++;
			}
			
		}
	}

	void emit() {
		for (auto e : all_slot) {
			static_cast<PSlot<F, _Ts...>*>( e )->setArgPtr(&_args);
			e->addSelftToWorkThread();
		}
	}
private:

	std::vector<slot_base*> all_slot;
	std::queue<std::tuple<_Ts...>> _args;

};



class PObject:protected PEventLoop{
public:
	PObject(PObject* parent = nullptr) :m_parent(parent) {}

	
	virtual ~PObject();

	


	template<typename _ST, typename _RT, typename ..._Ts>
	void disconnect(_ST* _OBJ_S, void(_ST::* _SIG)(_Ts...), _RT* __OBJ_R, void(_RT::* _SLOT)(_Ts...)) {
		std::pair<std::string, uint64_t> id = GetId(_OBJ_S, _SIG,__OBJ_R,_SLOT);
		_OBJ_S->uninstallSlot(id);
		
	}

	template<typename _ST, typename ...Ts>
	void disconnect(_ST* _OBJ_S, void(_ST::* _SIG)(Ts...),uint64_t id_) {
		std::pair<std::string, uint64_t> id = GetId(_OBJ_S, _SIG,id_);
		_OBJ_S->uninstallSlot(id);
	}

	template<typename _ST, typename _RT, typename ..._Ts>
	 void connect(_ST* _OBJ_S,void(_ST::* _SIG)(_Ts...),_RT* __OBJ_R,void(_RT::* _SLOT)(_Ts...))
	 {
		 std::pair<std::string, uint64_t> id = GetId(_OBJ_S, _SIG, __OBJ_R, _SLOT);
		
		PSlot < std::function<void(_Ts...)>, std::decay_t<_Ts>...>* slot_
		= __OBJ_R->template createSlot < std::function<void(_Ts...)>, std::decay_t<_Ts>...>(
			std::forward< std::function<void(_Ts...)>>(Bind(_SLOT, __OBJ_R)),
			id.second
			);

		_OBJ_S->template private_connect<std::function<void(_Ts...)>,std::decay_t<_Ts>...>(id.first, slot_);
	}

	template<typename _ST, typename F, typename ...Ts>
	void connect(_ST* _OBJ_S,void(_ST::* _SIG)(Ts...),uint64_t slot_id,F &&f) 
	{
		std::pair<std::string, uint64_t> id = GetId(_OBJ_S, _SIG,slot_id);
		
		PSlot<std::function<void(Ts...)>, std::decay_t<Ts>...> * slot_ =
			_OBJ_S->template createSlot<std::function<void(Ts...)>, std::decay_t<Ts>...>(std::forward<F>(f),slot_id);

		_OBJ_S->template private_connect<std::function<void(Ts...)>,std::decay_t<Ts>...>(id.first,slot_);
	}

	template<typename F, typename... _Ts>
	PSlot<F, std::decay_t<_Ts>...>* createSlot(F&& f,uint64_t sid) {
		PSlot<F, std::decay_t<_Ts>...>* n_slot = new PSlot<F, std::decay_t<_Ts>...>(std::forward<F>(f),sid);
		setSlotOf(n_slot);
		setPriority(n_slot);
		return n_slot;
	}
protected:
	virtual void setPriority(slot_base* s) {
		setEventPriority(s, (int)PEvent::CUSTM_FUNC);
	}


	signal_base* getSignalObject(const std::string& _SIG);

	void addSignalObject(const std::string& _SIG, signal_base* obj);

	template<typename F, typename ..._Ts>
	void private_connect(const std::string& _SIG, PSlot < F, std::decay_t<_Ts>...>* slot_t) {
		auto obj = getSignalObject(_SIG);

		if (obj != nullptr) {
			PSignal<F, std::decay_t<_Ts>...>* signal_t = static_cast<PSignal<F, std::decay_t<_Ts>...>*>(obj);
			signal_t->addSlot(slot_t);

		}
		else {

			PSignal<F, std::decay_t<_Ts>...>* signal_t = new PSignal< F, std::decay_t<_Ts>...>;
			signal_t->addSlot(slot_t);
			addSignalObject(_SIG, signal_t);
		}
	}


	template <typename _ST, typename ..._Ts>
	void emit(_ST* object, void(_ST::* _SIG)(_Ts...), _Ts... args) {
		std::pair<std::string, uint64_t> id = GetId(object, _SIG,0);
		auto sobj = getSignalObject(id.first);
		if (sobj != nullptr)
		{
			PSignal< std::function<void(_Ts...)>,std::decay_t<_Ts>...>* signal_t =
				static_cast<PSignal< std::function<void(_Ts...)>,std::decay_t<_Ts>...> *>(sobj);
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
	 _ED(0) __VA_ARGS__ 

#define _SIGNAL(className,signalName,one,tow,three)\
	void signalName(one){\
		emit<className,tow>(this,&className::signalName,three);\
	}

#define _SIGNAL_VOID(className,signalName)\
	void signalName(){\
		emit<className>(this,&className::signalName);\
	}


#endif // !_POBJECT_M_
