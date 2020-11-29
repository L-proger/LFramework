#pragma once

#include <cstdint>

namespace LFramework {

struct Guid {
    std::uint32_t data1;
    std::uint16_t data2;
    std::uint16_t data3;
    std::uint8_t  data4[8];

    constexpr bool operator ==(const Guid& rhs) const {
        return
                (data1 == rhs.data1) &&
                (data2 == rhs.data2) &&
                (data3 == rhs.data3) &&
                (*reinterpret_cast<const std::uint64_t*>(data4) == *reinterpret_cast<const std::uint64_t*>(rhs.data4));
    }

    constexpr bool operator !=(const Guid& rhs) const {
        return !(this->operator ==(rhs));
    }
};

}
