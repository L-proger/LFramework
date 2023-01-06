#pragma once

#include "../IUsbHostEndpoint.h"

namespace LFramework::USB {

class UsbHostEndpoint : public IUsbHostEndpoint {
public:
    UsbHostEndpoint(const USB::EndpointDescriptor& descriptor, int deviceFileDescriptor, std::uint32_t deviceCaps);
    std::shared_ptr<IUsbTransfer> transferAsync(void* buffer, size_t size) override;
private:
    int _deviceFileDescriptor;
    std::uint32_t _deviceCaps;

    std::shared_ptr<IUsbTransfer> readAsync(void* buffer, size_t size);
    std::shared_ptr<IUsbTransfer> writeAsync(void* buffer, size_t size);
};

}