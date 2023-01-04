#pragma once

#include "../UsbEndpoint.h"
#include "IUsbTransfer.h"
#include <memory>

namespace LFramework::USB {

class IUsbEndpoint : public UsbEndpoint {
public:
    IUsbEndpoint(const USB::EndpointDescriptor& descriptor):UsbEndpoint(descriptor){}
    virtual ~IUsbEndpoint() = default;
    virtual std::shared_ptr<IUsbTransfer> transferAsync(void* buffer, size_t size) = 0;
};

}