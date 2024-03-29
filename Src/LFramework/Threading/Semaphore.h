#pragma once

#include <LFramework/Detect/DetectOS.h>

#if LF_TARGET_OS == LF_OS_FREERTOS
#include "SemaphoreFreeRTOS.h"
#elif (LF_TARGET_OS == LF_OS_WINDOWS) || (LF_TARGET_OS == LF_OS_LINUX)
#include "SemaphoreStd.h"
#endif

