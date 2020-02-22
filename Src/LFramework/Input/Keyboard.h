#ifndef Keyboard_h__
#define Keyboard_h__

#include <Windows.h>
#include <cstdint>
#include <string>
#include "KeysStateManager.h"

class Keyboard {
public:
	friend class Input;
	Keyboard(HANDLE hdevice);
	
	HANDLE GetHandle() const;
	bool GetKeyState(uint32_t key_index) const;
	bool GetKeyPressed(uint32_t key_index) const;
	bool GetKeyReleased(uint32_t key_index) const;
private:
	void Strobe();
	void ApplyInput(RAWINPUT input);
	HANDLE _hdevice;
	std::wstring _name;
	uint32_t _function_keys_count;
	uint32_t _keys_count;
	uint32_t _indicators_count;

	KeysStateManager<0xff, Keyboard> _keys_manager;
};
#endif // Keyboard_h__
