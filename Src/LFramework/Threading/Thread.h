#pragma once

#include "LFrameworkConfig.h"

#ifdef LF_THREADING

#include <cstddef>
#include <cassert>
#include <functional>
#include <memory>

#include <LFramework/Threading/ThreadPlatform.h>

namespace LFramework::Threading {

enum class ThreadPriority{
	Idle,
	Low,
	BelowNormal,
	Normal,
	AboveNormal,
	High,
	Realtime,
	Error
};

class Thread {
public:
	using ID = ThreadID;

	explicit Thread(const char* name, size_t stackSize, ThreadPriority priority, std::function<void()>&& func);

	~Thread();
	Thread() noexcept;
	Thread(const Thread&) = delete;
	Thread(Thread&& other) noexcept;

	Thread& operator = (const Thread&) = delete;
	Thread& operator = (Thread&& other) noexcept;

	void swap(Thread& other);

	bool joinable() const noexcept;
	void join();
	//TODO: detach

	ID getId() const noexcept;
	//NativeHandleType nativeHandle() const noexcept;

	const char* getName() const;

	ThreadPriority getPriority() const;
	void setPriority(ThreadPriority priority) const;

	void suspend();
	void resume();
private:
	class Impl;
	Thread& moveTask(Thread& other);
	Impl* _impl = nullptr;
};

namespace ThisThread {
	Thread::ID getId();
	ThreadPriority getPriority();
	void setPriority(ThreadPriority priority);
	void sleepForMs(size_t milliseconds);
}

}

#endif //LF_THREADING

