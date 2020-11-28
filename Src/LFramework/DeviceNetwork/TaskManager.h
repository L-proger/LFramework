#pragma once

#include <MicroNetwork/Common/Packet.h>
#include "Device/ITaskContext.h"
#include <MicroNetwork/Common/IDataReceiver.h>

namespace MicroNetwork::Device {

    class Task : public Common::IDataReceiver {
	public:
		virtual ~Task() = default;
		virtual void run(ITaskContext* context) = 0;
	};

	class TaskManager {
	public:
		virtual ~TaskManager() = default;
		virtual Task* createTask() = 0;
		virtual void deleteTask(Task* task) = 0;
	};

}
