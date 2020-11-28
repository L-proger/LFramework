#pragma once

#include <LFramework/Threading/Thread.h>
#include "../Detect/DetectOS.h"


#if LF_TARGET_OS == LF_OS_FREERTOS
#include <LFramework/Threading/SemaphoreFreeRTOS.h>
#include "task.h"
#endif


namespace LFramework::Threading {

#if LF_TARGET_OS == LF_OS_FREERTOS
		static Thread::ID makeThreadId(TaskHandle_t handle) {
			return reinterpret_cast<Thread::ID>(handle);
		}

		static ThreadPriority makeThreadPriority(portBASE_TYPE osPriority) {
			return static_cast<ThreadPriority>(osPriority);
		}

		static portBASE_TYPE makeOsPriority(ThreadPriority priority) {
			return static_cast<portBASE_TYPE>(priority);
		}


		class Thread::Impl {
		public:
			explicit Impl(const char* name, size_t stackSize, ThreadPriority priority, std::function<void()>&& body) :_body(std::move(body)) {
				xTaskCreate(&Impl::staticTaskHandler, name, stackSize, this, (UBaseType_t)priority, &_rtosTaskHandle);
			}

			void join(){
				_joinSemaphore.take();
			}

			Thread::ID getId() const {
				return makeThreadId(_rtosTaskHandle);
			}

			TaskHandle_t getNativeHandle() const {
				return _rtosTaskHandle;
			}

			const char* getName() const {
				return pcTaskGetTaskName(_rtosTaskHandle);
			}
			void suspend() {
				vTaskSuspend(_rtosTaskHandle);
			}
			void resume() {
				vTaskResume(_rtosTaskHandle);
			}
			ThreadPriority getPriority() const {
				return makeThreadPriority(uxTaskPriorityGet(_rtosTaskHandle));
			}
			void setPriority(ThreadPriority priority) const {
				vTaskPrioritySet(_rtosTaskHandle, makeOsPriority(priority));
			}
		private:
			static void staticTaskHandler(void* param) {
				Impl* _this = (Impl*)param;
				_this->taskHandler();
			}
			void taskHandler() {
				_body();
				assert(_joinSemaphore.give());
				vTaskDelete(xTaskGetCurrentTaskHandle());
			}
			TaskHandle_t _rtosTaskHandle = nullptr;
			std::function<void()> _body;
			Threading::BinarySemaphore _joinSemaphore;
		};


	namespace ThisThread {
		Thread::ID getId() {
			return makeThreadId(xTaskGetCurrentTaskHandle());
		}
		ThreadPriority getPriority() {
			return makeThreadPriority(uxTaskPriorityGet(xTaskGetCurrentTaskHandle()));
		}
		void setPriority(ThreadPriority priority) {
			vTaskPrioritySet(xTaskGetCurrentTaskHandle(), makeOsPriority(priority));
		}
		void sleepForMs(size_t milliseconds){
			vTaskDelay(milliseconds);
		}
	}
#endif
}
