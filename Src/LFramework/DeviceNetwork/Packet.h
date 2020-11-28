#pragma once

#include <cstdint>
#include "../BitField.h"
#include <array>

namespace LFramework::DeviceNetwork {

union PacketDescription {
    std::uint8_t data;
    BitFieldMember<std::uint8_t, 0, 1> isService;
    BitFieldMember<std::uint8_t, 1, 7> reserved;
};

static_assert(sizeof(PacketDescription) == 1, "Invalid PacketDescription size");

struct PacketId {
    static constexpr std::uint8_t Bind = 0xff;
    static constexpr std::uint8_t Unbind = 0xfe;
    static constexpr std::uint8_t TaskStart = 0xfd;
    static constexpr std::uint8_t TaskStop = 0xfc;
};

struct PacketHeader {
    std::uint8_t id;
    std::uint8_t size;

    std::size_t fullSize() const {
    	return sizeof(PacketHeader) + size;
    }
};

struct MaxPacket {
       PacketHeader header;
       std::array<std::uint8_t, 256> payload;
   };

}
