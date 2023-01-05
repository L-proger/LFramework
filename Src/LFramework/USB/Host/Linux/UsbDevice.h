#pragma once

#include "../IUsbDevice.h"

#include <vector>
#include <memory>

namespace LFramework::USB {

class UsbDevice : public IUsbDevice {
public:
    UsbDevice(const std::string& path);
    std::shared_ptr<IUsbInterface> getInterface(std::size_t id) override;
    std::size_t getInterfaceCount() const override;
private:
    int _deviceFileDescriptor;
    std::vector<std::shared_ptr<IUsbInterface>> _interfaces;
    void enumerateInterfaces(const std::string& sysfsDevicePath);
};

}