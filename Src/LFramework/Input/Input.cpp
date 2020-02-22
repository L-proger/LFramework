#ifdef _WIN32

#include "Input.h"
#include <assert.h>
#include <iostream>
#include <string>
#include <algorithm>

#ifdef LF_INPUT

Input::Input() {
	auto hInst = GetModuleHandle(NULL);
	register_input_window_class(hInst);

	_input_window = CreateWindow(_class_name, TEXT("RawInputWindow"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInst, NULL);

	if (_input_window == NULL) {
		std::cout << "Failed to create input window!" << std::endl;
	}
	SetWindowLongPtr(_input_window, GWLP_USERDATA, (LONG_PTR)this);
	ShowWindow(_input_window, SW_HIDE);
	UpdateWindow(_input_window);

	enumerate_devices();

	RAWINPUTDEVICE Rid[2];

	Rid[0].usUsagePage = 0x01; //HID mouse
	Rid[0].usUsage = 0x02;
	Rid[0].dwFlags = RIDEV_INPUTSINK;
	Rid[0].hwndTarget = _input_window;

	Rid[1].usUsagePage = 0x01; //HID keyboard
	Rid[1].usUsage = 0x06;
	Rid[1].dwFlags = RIDEV_INPUTSINK;
	Rid[1].hwndTarget = _input_window;

	if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE) {
		std::cout << "Failed to register raw input devices!" << std::endl;
	}
}

std::shared_ptr<Input> Input::Instance() {
	static std::shared_ptr<Input> instance(new Input());
	return instance;
}

const std::vector<std::shared_ptr<Mouse>>& Input::mice() const{
	return _mice;
}

void Input::strobe(){

	MSG msg = { 0 };
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	for(auto mouse : _mice){
		mouse->Strobe();
	}
	for(auto kb : _keyboards){
		kb->Strobe();
	}
}

void Input::enumerate_devices() {
	_mice.clear();

	UINT numDevices;
	GetRawInputDeviceList(NULL, &numDevices, sizeof(RAWINPUTDEVICELIST));
	if (numDevices == 0) return;

	std::vector<RAWINPUTDEVICELIST> deviceList(numDevices);
	GetRawInputDeviceList(&deviceList[0], &numDevices, sizeof(RAWINPUTDEVICELIST));

	std::vector<wchar_t> deviceNameData;
	std::wstring deviceName;
	for (UINT i = 0; i < numDevices; ++i) {
		const RAWINPUTDEVICELIST& device = deviceList[i];
		if (device.dwType == RIM_TYPEMOUSE) {
			_mice.push_back(std::make_shared<Mouse>(device.hDevice));
		} else if (device.dwType == RIM_TYPEKEYBOARD) {
			_keyboards.push_back(std::make_shared<Keyboard>(device.hDevice));
		} else {
			std::cout << "HID device" << std::endl;
		}
	}
}

ATOM Input::register_input_window_class(HINSTANCE hinstance) {

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = &Input::wnd_proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hinstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _class_name;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	return RegisterClassEx(&wcex);
}

const std::vector<std::shared_ptr<Keyboard>>& Input::keyboards() const
{
	return _keyboards;
}

LRESULT CALLBACK Input::wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	auto input = (Input*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_INPUT: {
		UINT dwSize;
		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];

		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize) {
			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));
		}

		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEKEYBOARD) {
			auto device = std::find_if(input->_keyboards.begin(), input->_keyboards.end(), [raw](std::shared_ptr<Keyboard> m) {return m->GetHandle() == raw->header.hDevice; });
			if (device != input->_keyboards.end()) {
				(*device)->ApplyInput(*raw);
			}
		} else if (raw->header.dwType == RIM_TYPEMOUSE) {
			auto device = std::find_if(input->_mice.begin(), input->_mice.end(), [raw](std::shared_ptr<Mouse> m) {return m->GetHandle() == raw->header.hDevice; });
			if(device != input->_mice.end()){
				(*device)->ApplyInput(*raw);
			}
		}
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

#endif

#endif
