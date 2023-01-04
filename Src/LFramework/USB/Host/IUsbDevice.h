#pragma once

#include "IUsbInterface.h"
#include <cstdint>
#include <memory>

namespace LFramework::USB {

class IUsbDevice {
public:
    virtual std::shared_ptr<IUsbInterface> getInterface(std::size_t id) = 0;
    virtual std::size_t getInterfaceCount() const = 0;
};

}