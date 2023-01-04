#pragma once

#include <Windows.h>
#include <cstdint>
#include "KeysStateManager.h"
#include <string>

class Mouse {
public:
	static constexpr uint32_t maxButtonsCount = 5;
	friend class Input;
	Mouse(HANDLE handle);
	HANDLE GetHandle() const;
	bool GetKeyState(uint32_t key_index) const;
	bool GetKeyPressed(uint32_t key_index) const;
	bool GetKeyReleased(uint32_t key_index) const;
	int32_t GetDx() const;
	int32_t GetDy() const;
private:
	void Strobe();
	void ApplyInput(RAWINPUT input);
	LONG _deltax_integral;
	LONG _deltay_integral;
	LONG _deltax;
	LONG _deltay;
	KeysStateManager<maxButtonsCount, Mouse> _keys_manager;
	HANDLE _handle;
	std::wstring _name;
	uint32_t _buttons_count;
	bool _has_horizontal_wheel;
};

