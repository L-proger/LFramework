/*
 * Mcu.h
 *
 *  Created on: 19 July. 2017 ã.
 *      Author: l-pro
 */

#ifndef MCUFRAMEWORK_MCU_H_
#define MCUFRAMEWORK_MCU_H_

#include "LFrameworkConfig.h"
#include <cstddef>

#ifndef CMSIS_DEVICE_FILE
#error "CMSIS_DEVICE_FILE path not defined in McuConfig.h"
#else
#include CMSIS_DEVICE_FILE
#endif

namespace LFramework {
namespace Mcu {
	inline size_t getCurrentInterruptHandlerIndex(){
		return __get_IPSR();
	}

	inline bool isHandlingInterrupt() {
		return getCurrentInterruptHandlerIndex() != 0;
	}

	inline bool isDebuggerAttached() {
		return (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) != 0;
	}

	inline void setBreakpoint() {
		if(isDebuggerAttached){
			__BKPT(0);
		}
	}
}
}

#endif /* MCUFRAMEWORK_MCU_H_ */
