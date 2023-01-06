#pragma once

#include "IUsbHostEndpoint.h"

namespace LFramework::USB {

class IUsbInterface {
public:
    virtual IUsbHostEndpoint* getEndpoint(bool isInEndpoint, uint8_t id) = 0;
    virtual const IUsbHostEndpoint* getEndpoint(bool isInEndpoint, uint8_t id) const = 0;
    virtual const USB::InterfaceDescriptor& getInterfaceDescriptor() const = 0;
};

} 