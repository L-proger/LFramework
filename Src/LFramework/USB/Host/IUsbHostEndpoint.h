#pragma once

#include "../UsbEndpoint.h"
#include "IUsbTransfer.h"
#include <memory>

namespace LFramework::USB {

class IUsbHostEndpoint : public UsbEndpoint {
public:
    IUsbHostEndpoint(const USB::EndpointDescriptor& descriptor):UsbEndpoint(descriptor){}
    virtual ~IUsbHostEndpoint() = default;
    virtual std::shared_ptr<IUsbTransfer> transferAsync(void* buffer, size_t size) = 0;
};

}