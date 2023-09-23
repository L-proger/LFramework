
#ifdef _WIN32

#include "Mouse.h"

#include <iostream>


Mouse::Mouse(HANDLE handle) :_handle(handle){
	_deltax = _deltay = _deltax_integral = _deltay_integral = 0;
	
	UINT dataSize;
	GetRawInputDeviceInfo(_handle, RIDI_DEVICENAME, nullptr, &dataSize);
	if (dataSize) {
		_name.resize(dataSize);
		UINT result = GetRawInputDeviceInfo(_handle, RIDI_DEVICENAME, &_name[0], &dataSize);
		if (result != UINT_MAX) {
			_name.assign(_name.begin(), _name.end());
		}
	}

	RID_DEVICE_INFO deviceInfo;
	deviceInfo.cbSize = sizeof deviceInfo;
	dataSize = sizeof deviceInfo;
	UINT result = GetRawInputDeviceInfo(_handle, RIDI_DEVICEINFO, &deviceInfo, &dataSize);
	if (result != UINT_MAX) {
		_buttons_count = deviceInfo.mouse.dwNumberOfButtons;
		_has_horizontal_wheel = deviceInfo.mouse.fHasHorizontalWheel == TRUE;
	}
}

HANDLE Mouse::GetHandle() const{
	return _handle;
}

bool Mouse::GetKeyState(uint32_t key_index) const{
	return _keys_manager._key_states[key_index];
}

bool Mouse::GetKeyPressed(uint32_t key_index) const{
	return _keys_manager._key_pressed[key_index];
}

bool Mouse::GetKeyReleased(uint32_t key_index) const{
	return _keys_manager._key_released[key_index];
}

void Mouse::Strobe() {
	_deltax = _deltax_integral;
	_deltax_integral = 0;
	_deltay = _deltay_integral;
	_deltay_integral = 0;
	_keys_manager.strobe();
}

int32_t Mouse::GetDx() const{
	return _deltax;
}

int32_t Mouse::GetDy() const{
	return _deltay;
}

void Mouse::ApplyInput(RAWINPUT input) {
	//input.data.mouse.lLastX
	_deltax_integral += input.data.mouse.lLastX;
	_deltay_integral += input.data.mouse.lLastY;

	RAWMOUSE data = input.data.mouse;
	for (uint32_t i = 0; i < maxButtonsCount; ++i){
		if((data.ulButtons & (1 << (i << 1))) != 0){
			_keys_manager._key_states[i] = true;
		}
		if ((data.ulButtons & (2 << (i << 1))) != 0) {
			_keys_manager._key_states[i] = false;
		}
	}
}
#endif

