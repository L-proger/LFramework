#pragma once

#include <MicroNetwork/Common/IDataReceiver.h>

namespace MicroNetwork::Device {

    class ITaskContext : public Common::IDataReceiver {
	public:
		virtual ~ITaskContext() = default;
		virtual void readPackets() = 0;
		virtual bool isExitRequested() = 0;
	};

}
