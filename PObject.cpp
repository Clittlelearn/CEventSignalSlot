#include "PObject.h"
 

 
PObject::~PObject()
{
	if (!object_loop) {
		errorL("object_loop is nullptr");
		return ;
	}

	for (auto s : l_signal_slot) {
		auto sig=__ALL_SIGNALS_.find(s.first);
		if (sig!=__ALL_SIGNALS_.end()) {
			auto sl = sig->second;
			sl->deleteSlot(s.second);
			if (sl->isEmpty()) {
				__ALL_SIGNALS_.erase(s.first);
				delete sl;
			}
			
		}
	}

}

signal_base* PObject::getSignalObject(const std::string& _SIG)
{
	if (!object_loop) {
		errorL("object_loop is nullptr");
		return nullptr;
	}
	auto iter = __ALL_SIGNALS_.find(_SIG);
	if (iter != __ALL_SIGNALS_.end()) {
		return iter->second;
	}
	else {
		return nullptr;
	}
}

void PObject::addSignalObject(const std::string& _SIG, signal_base* obj)
{
	if (!object_loop) {
		errorL("object_loop is nullptr");
		return ;
	}
	auto iter = __ALL_SIGNALS_.find(_SIG);
	if (iter != __ALL_SIGNALS_.end()) {
		return;
	}
	else {
		__ALL_SIGNALS_[_SIG] = obj;
	}
}

void PObject::clearSignalObject(const std::string& _SIG)
{
	

}

void PObject::uninstallSlot(std::pair< std::string, uint64_t>& pa)
{
	
	for (auto s = l_signal_slot.begin(); s != l_signal_slot.end(); s++) {
		if ((*s).first == pa.first && (*s).second==pa.second) {
			l_signal_slot.erase(s);
		}
	}
}

void PObject::installSlot(std::pair< std::string,uint64_t> & pa)
{
	l_signal_slot.push_back(pa);
}
