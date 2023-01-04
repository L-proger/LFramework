#pragma once

#include <cstdint>

namespace LFramework::USB {

class IUsbTransfer {
public:
    virtual ~IUsbTransfer() = default;
    virtual std::size_t wait() = 0;
};

}