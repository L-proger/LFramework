#pragma once

#include <cstdint>


namespace LFramework {

struct Guid {
    std::uint32_t data1;
    std::uint32_t data2_3;
    std::uint32_t data4_0;
    std::uint32_t data4_1;

    //std::uint16_t data2;
    //std::uint16_t data3;
    //std::uint8_t  data4[8];

    Guid() = default;

    Guid(std::uint32_t data1, std::uint32_t data2_3, std::uint32_t data4_0, std::uint32_t data4_1) : data1(data1), data2_3(data2_3), data4_0(data4_0), data4_1(data4_1) {}

    constexpr bool operator ==(const Guid& rhs) const {
        return
            (data1 == rhs.data1) &&
            (data2_3 == rhs.data2_3) &&
            (data4_0 == rhs.data4_0) &&
            (data4_1 == rhs.data4_1);
    }

    constexpr bool operator !=(const Guid& rhs) const {
        return !(this->operator ==(rhs));
    }

	bool isZero() const {
		return (data1 == 0) && (data2_3 == 0) && (data4_0 == 0) && (data4_1 == 0);
	}

	Guid& operator =(const Guid& right) {
        data1 = right.data1;
        data2_3 = right.data2_3;
        data4_0 = right.data4_0;
        data4_1 = right.data4_1;
		return *this;
	}
};

}

