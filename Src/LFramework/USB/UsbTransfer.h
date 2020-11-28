#pragma once

#include <cstdint>

namespace LFramework::USB {

class UsbTransfer {
public:
	typedef void(*TransferCompleteCallback)(UsbTransfer*, bool);
	void* buffer = nullptr;
	std::uint32_t size = 0;
	std::uint32_t actualSize = 0;
	void* context = nullptr;
	TransferCompleteCallback callback = nullptr;
};

}
