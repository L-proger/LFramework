#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <functional>
#include <thread>
#include <Windows.h>
#include <future>
#include <LFramework/Threading/Semaphore.h>


class UsbService : public IUsbService {
public:
    UsbHostDeviceInfo getUsbDeviceInfo(const std::string& path);
    std::vector<UsbHostDeviceInfo> enumerateDevices();
    bool startEventsListening(std::function<void()> deviceChangeCallback);
    void stopEventsListening();
private:
    static DWORD WINAPI eventsThreadProc(LPVOID lpParam);
    HANDLE _threadHandle = {};
    DWORD _threadId = 0;
    std::promise<bool> _initPromise;
    std::function<void()> _deviceChangeCallback;
};
