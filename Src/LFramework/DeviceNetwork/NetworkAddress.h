#pragma once

#include <cstdint>

namespace LFramework::DeviceNetwork {

	class NetworkAddress {
	public:
		static constexpr NetworkAddress Root = {0x00};
		static constexpr NetworkAddress Broadcast = {0xff};
		std::uint8_t value = Root.value;
	};
}
