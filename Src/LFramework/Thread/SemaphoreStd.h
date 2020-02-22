#ifndef SemaphoreStd_h__
#define SemaphoreStd_h__

#include <mutex>
#include <condition_variable>

class Semaphore {
public:
	explicit Semaphore(size_t initialCount = 0):_count(initialCount){}

	void take(size_t count = 1) {
		std::unique_lock<std::mutex> lk(_mutex);
		_conditional.wait(lk, [=] { return _count >= count; });
		_count -= count;
	}

	bool tryTake(size_t count = 1) {
		std::lock_guard<std::mutex> lk(_mutex);
		if (_count >= count) {
			_count -= count;
			return true;
		}
		return false;
	}

	void give(size_t count = 1) {
		std::lock_guard<std::mutex> lk(_mutex);
		if(count == 1) {
			++_count;
			_conditional.notify_one();
		}else {
			_count += count;
			_conditional.notify_all();
		}
	}

	size_t available() const {
		std::lock_guard<std::mutex> lk(_mutex);
		return _count;
	}

private:
	size_t _count;
	mutable std::mutex _mutex;
	mutable std::condition_variable _conditional;
};

#endif // SemaphoreStd_h__
