#pragma once

#include <mutex>
#include <condition_variable>
#include <atomic>

namespace LFramework::Threading {

    class CountingSemaphore {
	public:
        static constexpr std::size_t InfiniteTimeout = -1;
        CountingSemaphore(std::size_t maxCount, std::size_t initialCount)  :_maxCount(maxCount), _count(initialCount) {

        }
		bool take() {
            std::unique_lock<std::mutex> lk(_mutex);
			_conditional.wait(lk, [this] { return _count > 0; });
			--_count;
			return true;
		}

        bool take(std::size_t milliseconds) {
            std::unique_lock<std::mutex> lk(_mutex);
            if (_conditional.wait_for(lk, ::std::chrono::milliseconds(milliseconds), [this] { return _count > 0; })) {
				--_count;
				return true;
			}
			return false;
		}

		bool tryTake() {
            std::lock_guard<::std::mutex> lk(_mutex);
			if (_count > 0) {
				--_count;
				return true;
			}
			else {
				return false;
			}
		}

        bool give(std::size_t count) {
            if(count == 0){
                return true;
            }
            std::lock_guard<::std::mutex> lk(_mutex);
            if ((_count + count) <= _maxCount) {
                _count += count;
                if(count > 1){
                    _conditional.notify_all();
                }else{
                    _conditional.notify_one();
                }
                return true;
			}
            return false;
		}

        bool give() {
            std::lock_guard<::std::mutex> lk(_mutex);
            if (_count < _maxCount) {
                ++_count;
                _conditional.notify_one();
                return true;
            }
            return false;
        }

        std::size_t messagesWaiting() const {
            return _count.load(std::memory_order_relaxed);
		}

	private:
        std::size_t _maxCount = -1;
        std::atomic<std::size_t> _count = 0;
        std::mutex _mutex;
        std::condition_variable _conditional;
	};

    class BinarySemaphore : public CountingSemaphore {
    public:
        BinarySemaphore(bool startWithValue = false) : CountingSemaphore(1, startWithValue ? 1 : 0){ }
    };

}
