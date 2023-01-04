#pragma once

#include "IUsbEndpoint.h"

namespace LFramework::USB {

class IUsbInterface {
public:
    virtual IUsbEndpoint* getEndpoint(bool isInEndpoint, uint8_t id) = 0;
    virtual const IUsbEndpoint* getEndpoint(bool isInEndpoint, uint8_t id) const = 0;
    virtual const USB::InterfaceDescriptor& getInterfaceDescriptor() const = 0;
};

} 