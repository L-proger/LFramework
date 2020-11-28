#pragma once


#include "../MCU/Mcu.h"
#include <cassert>
#include <cstdint>

#include "FreeRTOS.h"
#include "portable.h"
#include "queue.h"
#include "semphr.h"


namespace LFramework::Threading {

class Semaphore {
public:
	~Semaphore(){
		vSemaphoreDelete(_handle);
	}

	size_t messagesWaiting() const {
		return (size_t)uxQueueMessagesWaiting(_handle);
	}

	bool take(TickType_t wait = portMAX_DELAY) {
		if (LFramework::Mcu::isHandlingInterrupt()) {
			portBASE_TYPE taskWoken = pdFALSE;
			if (xSemaphoreTakeFromISR(_handle, &taskWoken) != pdTRUE) {
				return false;
			}
			portEND_SWITCHING_ISR(taskWoken);
			return true;
		}  else {
			return xSemaphoreTake(_handle, wait) == pdTRUE;
		}
	}

	bool give() {
		if (LFramework::Mcu::isHandlingInterrupt()) {
			portBASE_TYPE taskWoken = pdFALSE;
			if (xSemaphoreGiveFromISR(_handle, &taskWoken) != pdTRUE) {
				return false;
			}
			portEND_SWITCHING_ISR(taskWoken);
			return true;
		} else {
			return xSemaphoreGive(_handle) == pdTRUE;
		}
	}

protected:
	SemaphoreHandle_t _handle;
};

class BinarySemaphore : public Semaphore{
public:
	BinarySemaphore(){
		_handle = xSemaphoreCreateBinary();
		assert(_handle != nullptr);
	}
};

class CountingSemaphore : public Semaphore{
public:
	CountingSemaphore(UBaseType_t maxCount, UBaseType_t initialCount){
		_handle = xSemaphoreCreateCounting(maxCount, initialCount);
		assert(_handle != nullptr);
	}
};

}

