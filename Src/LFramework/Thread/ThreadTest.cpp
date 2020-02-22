#include "Thread.h"
#include "../UnitTest/UnitTest.h"

#ifdef LF_THREADING

using namespace LFramework;

void test(int& counter) {
	counter = 100500;
}

TEST(Thread_Create) {
	int counter = 0;
	Thread t;
	TEST_TRUE(t.joinable() == false);

	t = Thread("123", 123, ThreadPriority::Normal, [&](){
		test(counter);
	});
	t.join();

	TEST_TRUE(counter == 100500);
	TEST_TRUE(!t.joinable());
}

#endif
