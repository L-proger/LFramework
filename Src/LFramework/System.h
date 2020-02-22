#ifndef System_h__
#define System_h__

#include "LFrameworkConfig.h"
#include <cstddef>

#include "Detect/DetectOS.h"


#if LF_TARGET_OS == LF_OS_FREERTOS
#include "SystemArmCortexM.h"
#elif LF_TARGET_OS == LF_OS_WINDOWS
#include "SystemWindows.h"
#endif


#endif // System_h__
