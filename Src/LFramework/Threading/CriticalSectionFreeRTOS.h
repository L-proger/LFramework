#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "stm32f746xx.h"

namespace LFramework::Threading {

	class CriticalSection {
	public:
		CriticalSection() {
			_status = lock();
		}

		~CriticalSection() {
			unlock();
		}

		void unlock() {
			unlock(_status);
		}

		static size_t getCurrentInterruptNumber() {
			return __get_IPSR();
		}

		static bool isInterruptHandling() {
			return getCurrentInterruptNumber() != 0;
		}

		static uint32_t lock() {
            if (isInterruptHandling()) {
				return taskENTER_CRITICAL_FROM_ISR();
			}
			else {
				taskENTER_CRITICAL();
			}
			return 0;
		}

		static void unlock(uint32_t irqStatus) {
            if (isInterruptHandling()) {
				taskEXIT_CRITICAL_FROM_ISR(irqStatus);
			}
			else {
				taskEXIT_CRITICAL();
			}
		}

	private:
		uint32_t _status = 0;
	};
}
