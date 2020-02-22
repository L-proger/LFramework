#include "../UnitTest/UnitTest.h"

#ifdef LF_THREADING

#include "SemaphoreStd.h"
#include "Thread.h"

#include <vector>

using namespace LFramework;


TEST(SemaphoreStd_SyncUsage) {
	Semaphore s0;

	TEST_TRUE(s0.available() == 0);
	s0.give();
	TEST_TRUE(s0.available() == 1);
	s0.give(9);
	TEST_TRUE(s0.available() == 10);

	s0.take(0);
	TEST_TRUE(s0.available() == 10);
	s0.take();
	TEST_TRUE(s0.available() == 9);
	s0.take(9);
	TEST_TRUE(s0.available() == 0);
}

TEST(SemaphoreStd_AsyncUsage) {
	static constexpr size_t initialCount = 25;

	Semaphore resources(initialCount);

	Thread consumerThread0 = Thread("", 0, ThreadPriority::Normal, [&resources]() {
		for(size_t i = 0; i < 100; ++i) {
			resources.take();
			ThisThread::sleepForMs(1);
		}
	});

	Thread consumerThread1 = Thread("", 0, ThreadPriority::Normal, [&resources]() {
		for (size_t i = 0; i < 10; ++i) {
			resources.take(10);
			ThisThread::sleepForMs(2);
		}
	});

	for(size_t i = 0; i < (200 - initialCount); i += 5) {
		resources.give(5);
	}

	consumerThread0.join();
	consumerThread1.join();
}

#endif
