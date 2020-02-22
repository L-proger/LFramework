#ifndef ThreadStd_h__
#define ThreadStd_h__

#include "LFrameworkConfig.h"

#if defined(LF_THREADING) && defined(LF_THREAD_STD)

#include "Thread.h"
#include <thread>
#include <string>
#include <chrono>

namespace LFramework {
	class Thread::Impl {
	public:
		explicit Impl(const char* name, size_t stackSize, ThreadPriority priority, std::function<void()>&& body):_name(name){
			_thread = std::thread(body);
		}
		void join() {
			_thread.join();
		}
		Thread::ID getId() const {
			return _thread.get_id();
		}
		const char* getName() const {
			return _name.c_str();
		}
		void suspend() {
				
		}
		void resume() {
				
		}
		ThreadPriority getPriority() const {
			return ThreadPriority::Normal;
		}
		void setPriority(ThreadPriority priority) const {
				
		}
	private:
		std::string _name;
		std::thread _thread;
	};

	namespace ThisThread {
		inline Thread::ID getId() {
			return std::this_thread::get_id();
		}
		void sleepForMs(size_t milliseconds) {
		
			std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
		}
		/*ThreadPriority getPriority() {
			return Private::makeThreadPriority(uxTaskPriorityGet(xTaskGetCurrentTaskHandle()));
		}
		void setPriority(ThreadPriority priority) {
			vTaskPrioritySet(xTaskGetCurrentTaskHandle(), Private::makeOsPriority(priority));
		}*/
	}
}

#endif

#endif // ThreadStd_h__
