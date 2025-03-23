#pragma once

#include <cstdint>
#include <cstddef>

#ifndef CMSIS_DEVICE_FILE
#error "CMSIS_DEVICE_FILE path not defined in McuConfig.h"
#else
#include CMSIS_DEVICE_FILE
#endif

namespace LFramework {
	namespace System {
		inline size_t getCurrentInterruptHandlerIndex() {
			return __get_IPSR();
		}

		inline bool isHandlingInterrupt() {
			return getCurrentInterruptHandlerIndex() != 0;
		}

		inline bool isDebuggerAttached() {
			return (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) != 0;
		}

		inline void setBreakpoint() {
			if (isDebuggerAttached) {
				__BKPT(0);
			}
		}
	}
}
