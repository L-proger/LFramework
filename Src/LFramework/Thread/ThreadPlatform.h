#pragma once

#include "LFrameworkConfig.h"
#include "../Detect/DetectOS.h"

#if defined(LF_THREADING)


#if LF_TARGET_OS == LF_OS_FREERTOS
	#include "FreeRTOS.h"
	#include "task.h"

namespace LFramework {
	using ThreadID = TaskHandle_t;
}
#else
#include <thread>
namespace LFramework {
	using ThreadID = std::thread::id;
}
#endif


#endif //LF_THREADING
