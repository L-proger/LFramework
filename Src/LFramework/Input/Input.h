#pragma once

#include <memory>
#include <Windows.h>
#include <vector>
#include "Mouse.h"
#include "Keyboard.h"

class Input {
public:
	Input();
	static std::shared_ptr<Input> Instance();
	const std::vector<std::shared_ptr<Mouse>>& mice() const;
	const std::vector<std::shared_ptr<Keyboard>>& keyboards() const;
	void strobe();
private:
	void enumerate_devices();
	ATOM register_input_window_class(HINSTANCE hinstance);
	static LRESULT CALLBACK wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HWND _input_window;
	const TCHAR* _class_name = TEXT("RawInputWindow");
	std::vector<std::shared_ptr<Mouse>> _mice;
	std::vector<std::shared_ptr<Keyboard>> _keyboards;
};
