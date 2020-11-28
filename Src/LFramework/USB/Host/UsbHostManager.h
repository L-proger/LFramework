#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct UsbHostDeviceInfo {
    uint16_t vid;
    uint16_t pid;
    std::string serialNumber;
    std::string path;
};

class UsbHostManager {
public:
    static UsbHostDeviceInfo getUsbDeviceInfo(const std::string& path);
    static std::vector<UsbHostDeviceInfo> enumerateDevices();
};
