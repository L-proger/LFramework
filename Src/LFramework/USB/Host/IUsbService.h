#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <functional>

namespace LFramework::USB {

struct UsbDeviceInfo {
    std::uint16_t vid;
    std::uint16_t pid;
    std::string serialNumber;
    std::string path;
};

class IUsbService {
public:
    virtual ~IUsbService() = default; 
    virtual UsbDeviceInfo getUsbDeviceInfo(const std::string& path) = 0;
    virtual std::vector<UsbDeviceInfo> enumerateDevices() = 0;
    virtual bool startEventsListening(std::function<void()> deviceChangeCallback) = 0;
    virtual void stopEventsListening() = 0;
};

}