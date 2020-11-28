#pragma once

#include "Packet.h"
#include "Device/ITaskContext.h"
#include "IDataReceiver.h"

namespace LFramework::DeviceNetwork {

	class Task : public IDataReceiver {
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
