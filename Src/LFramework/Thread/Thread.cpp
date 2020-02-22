/*
 * Thread.cpp
 *
 *  Created on: 19 July. 2017 ã.
 *      Author: l-pro
 */

#include "Thread.h"

#if defined(LF_THREADING)

#include "ThreadFreeRTOS.h"
#include "ThreadStd.h"


namespace LFramework {

	Thread::Thread(const char* name, size_t stackSize, ThreadPriority priority, std::function<void()>&& func) {
		_impl = new Thread::Impl(name, stackSize, priority, std::move(func));
	}


	Thread::Thread() noexcept = default;

	Thread::Thread(Thread&& other) noexcept{
		moveTask(other);
	}
	
	Thread::~Thread() {
		assert(!joinable());
		//_impl.reset();
	}
	bool Thread::joinable() const  noexcept {
		return _impl != nullptr;
	}
	void Thread::join() {
		assert(joinable());
		assert(ThisThread::getId() != getId()); //Can't join self
		_impl->join();
		delete _impl;
		_impl = nullptr;
	}
	void Thread::swap(Thread& other) {
		std::swap(_impl, other._impl);
	}
	/*Thread::NativeHandleType Thread::nativeHandle() const {
		assert(joinable());
		return _impl->getNativeHandle();
	}*/
	Thread::ID Thread::getId() const noexcept {
		if (_impl != nullptr) {
			return _impl->getId();
		}
		return Thread::ID{};
	}
	ThreadPriority Thread::getPriority() const {
		assert(joinable());
		return _impl->getPriority();
	}
	void Thread::setPriority(ThreadPriority priority) const {
		assert(joinable());
		_impl->setPriority(priority);
	}
	void Thread::suspend() {
		assert(joinable());
		_impl->suspend();
	}
	void Thread::resume() {
		assert(joinable());
		_impl->resume();
	}
	Thread& Thread::operator = (Thread&& other) noexcept {
		return moveTask(other);
	}
	const char* Thread::getName() const {
		assert(joinable());
		return _impl->getName();
	}

	Thread& Thread::moveTask(Thread& other) {
		assert(!joinable());
		_impl = std::move(other._impl);
		other._impl = nullptr;
		return *this;
	}

} //LFramework

#endif //LF_THREADING
