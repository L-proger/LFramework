#pragma once

#include <LFramework/Detect/DetectOS.h>

#if LF_TARGET_OS == LF_OS_FREERTOS
#include "CriticalSectionFreeRTOS.h"
#elif (LF_TARGET_OS == LF_OS_WINDOWS) || (LF_TARGET_OS == LF_OS_LINUX)
#include "CriticalSectionStd.h"
#endif

