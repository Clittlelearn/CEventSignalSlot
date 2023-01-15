#include "PObject.h"
 

 
PObject::~PObject()
{
	if (!object_loop) {
		errorL("object_loop is nullptr");
		return ;
	}

	for (auto s : __ALL_SIGNALS_) {
		delete s.second;
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



void PObject::uninstallSlot(std::pair< std::string, uint64_t>& pa)
{
	
	auto signal_ = __ALL_SIGNALS_.find(pa.first);
	if (signal_ != __ALL_SIGNALS_.end()) {
		signal_->second->deleteSlot(pa.second);
		if (signal_->second->isEmpty()) {
			__ALL_SIGNALS_.erase(signal_->first);
		}
	}
}


