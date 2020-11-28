#pragma once

#include "Packet.h"

namespace LFramework::DeviceNetwork {

	class IDataReceiver {
	public:
		virtual ~IDataReceiver() = default;
		virtual bool packet(PacketHeader header, const void* data) = 0;
	};

}
