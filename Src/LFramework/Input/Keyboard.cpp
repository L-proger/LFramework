#ifdef _WIN32

#include "Keyboard.h"
#include <iostream>

#ifdef LF_INPUT

Keyboard::Keyboard(HANDLE hdevice) :_hdevice(hdevice) {
	UINT dataSize;
	GetRawInputDeviceInfo(hdevice, RIDI_DEVICENAME, nullptr, &dataSize);
	if (dataSize) {
		std::wstring name_data;
		name_data.resize(dataSize);
		UINT result = GetRawInputDeviceInfo(hdevice, RIDI_DEVICENAME, &name_data[0], &dataSize);
		if (result != UINT_MAX) {
			_name = name_data;
		}
	}

	RID_DEVICE_INFO deviceInfo;
	deviceInfo.cbSize = sizeof deviceInfo;
	dataSize = sizeof deviceInfo;
	UINT result = GetRawInputDeviceInfo(hdevice, RIDI_DEVICEINFO, &deviceInfo, &dataSize);
	_function_keys_count = deviceInfo.keyboard.dwNumberOfFunctionKeys;
	_keys_count = deviceInfo.keyboard.dwNumberOfKeysTotal;
	_indicators_count = deviceInfo.keyboard.dwNumberOfIndicators;
}

void Keyboard::Strobe() {
	_keys_manager.strobe();
}

HANDLE Keyboard::GetHandle() const {
	return _hdevice;
}

bool Keyboard::GetKeyState(uint32_t key_index) const {
	return _keys_manager._key_states[key_index];
}

bool Keyboard::GetKeyPressed(uint32_t key_index) const {
	return _keys_manager._key_pressed[key_index];
}

bool Keyboard::GetKeyReleased(uint32_t key_index) const {
	return _keys_manager._key_released[key_index];
}

void Keyboard::ApplyInput(RAWINPUT input) {
	if (input.data.keyboard.Flags == RI_KEY_BREAK)
		_keys_manager._key_states[input.data.keyboard.VKey] = false;
	else
		_keys_manager._key_states[input.data.keyboard.VKey] = true;
}
#endif

#endif
