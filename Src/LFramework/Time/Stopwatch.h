#ifndef Stopwatch_h__
#define Stopwatch_h__

#include <chrono>
#include <cstdint>

namespace LFramework {
	class Stopwatch {
	public:
		Stopwatch() {
			Reset();
		}
		typedef std::chrono::high_resolution_clock clock_t;

		void Reset() {
			_startTime = clock_t::now();
		}
		clock_t::duration Check() {
			_lastDelta = clock_t::now() - _startTime;
			return _lastDelta;
		}
		std::chrono::milliseconds CheckMs() {
			return std::chrono::duration_cast<std::chrono::milliseconds>(Check());
		}
		clock_t::duration GetLastDelta() {
			return _lastDelta;
		}
	private:
		clock_t::time_point _startTime;
		clock_t::duration _lastDelta;
	};
}

#endif // Stopwatch_h__