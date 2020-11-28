#pragma once

#include <initguid.h>
#include <Usbiodef.h>
#include <Windows.h>
#include <stdexcept>
#include <Dbt.h>
#include <functional>

class UsbNotifyWindow {
public:
    UsbNotifyWindow(std::function<void()> deviceChangeCallback) : _callback(deviceChangeCallback) {
        HINSTANCE moduleHandle = GetModuleHandleA(nullptr);
        registerWindowClass(moduleHandle);
        _windowHandle = CreateWindowExA(0, windowClassName(), nullptr, 0, 0, 0, 0, 0, {}, {}, moduleHandle, nullptr);
        if(_windowHandle == NULL){
            throw std::runtime_error("UsbNotifyWindow::ctor failed to create window");
        }
        SetWindowLongPtrA(_windowHandle, GWLP_USERDATA, LONG_PTR(this));
    }

    ~UsbNotifyWindow() {
        UnregisterDeviceNotification(_deviceNotification);
        DestroyWindow(_windowHandle);
    }

    void processMessages() {
        MSG msg{};
        while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    }
private:
    HWND _windowHandle = {};
    HDEVNOTIFY _deviceNotification = {};
    std::function<void()> _callback;

    static const char* windowClassName() {
        return "LUsbNotifyWindow";
    }

    void onDeviceChange() {
        _callback();
    }

    static LRESULT CALLBACK windowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
        auto _this = reinterpret_cast<UsbNotifyWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        if (Msg == WM_DEVICECHANGE && (_this != nullptr)) {
            _this->onDeviceChange();
        }
        return DefWindowProcA(hWnd, Msg, wParam, lParam);
    }

    void registerDeviceNotification() {
        DEV_BROADCAST_DEVICEINTERFACE notificationFilter{};
        ZeroMemory( &notificationFilter, sizeof(notificationFilter) );
        notificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
        notificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
        notificationFilter.dbcc_classguid = GUID_DEVINTERFACE_USB_DEVICE;
        _deviceNotification = RegisterDeviceNotificationA(_windowHandle, &notificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE );
        if(_deviceNotification == nullptr){
            throw std::runtime_error("UsbNotifyWindow::registerDeviceNotification failed");
        }
    }

    static ATOM registerWindowClass(HINSTANCE moduleHandle) {
        WNDCLASSEXA wndClass = {};
        wndClass.cbSize = sizeof(wndClass);
        wndClass.hInstance = moduleHandle;
        wndClass.lpszClassName = windowClassName();
        wndClass.lpfnWndProc = &UsbNotifyWindow::windowProc;
        auto result = RegisterClassExA(&wndClass);
        if(result == 0){
            auto err = GetLastError();
            if(err != ERROR_CLASS_ALREADY_EXISTS){
                throw std::runtime_error("UsbNotifyWindow::registerWindowClass failed");
            }
        }
        return result;
    }
};
