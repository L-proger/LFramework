#pragma once

#include "UsbTransfer.h"

namespace LFramework::USB {

class UsbPollingTransfer : public LFramework::USB::UsbTransfer {
public:
	enum class State {
		Pending,
		Success,
		Fail
	};

	UsbPollingTransfer(){
		callback = &UsbPollingTransfer::transferComleteCallbackStatic;
	}
	void reset(){
		_state = State::Pending;
		size = 0;
		actualSize = 0;
	}
	State getState() const {
		return _state;
	}
private:
	State _state = State::Pending;
	void transferComleteCallback(bool success){
		_state = success ? State::Success : State::Fail;
	}

	static void transferComleteCallbackStatic(LFramework::USB::UsbTransfer* _this, bool success){
		static_cast<UsbPollingTransfer*>(_this)->transferComleteCallback(success);
	}
};

}
