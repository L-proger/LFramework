#pragma once

#include <cstdint>

namespace LFramework {

struct Guid {
    std::uint32_t data1;
    std::uint16_t data2;
    std::uint16_t data3;
    std::uint8_t  data4[8];
};

}
