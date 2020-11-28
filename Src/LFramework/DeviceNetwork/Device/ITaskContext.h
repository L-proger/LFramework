#pragma once

#include "../IDataReceiver.h"

namespace LFramework::DeviceNetwork {

	class ITaskContext : public IDataReceiver {
	public:
		virtual ~ITaskContext() = default;
		virtual void readPackets() = 0;
		virtual bool isExitRequested() = 0;
	};

}
