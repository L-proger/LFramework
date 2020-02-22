#pragma once

#include "../Containers/LinkedList.h"

namespace LFramework { namespace Testing {

	LinkedList& getTestsList();
	void runAllTests();

	struct TestResult {
		bool success = false;
		const char* message = nullptr;

		operator bool() const {
			return success;
		}
	};

	typedef void(*TestFunction)(TestResult& result);

	struct Test : public LinkedListItem {
		const char* name = nullptr;

		Test(const char* name, TestFunction testFunc): name(name), _test(testFunc){
			getTestsList().pushBack(this);
		}

		TestResult run(){
			TestResult result{};
			result.success = true;
			_test(result);
			return result;
		}
	private:
		TestFunction _test;
	};


#define TEST_FUNC_NAME(name) test_##name

#define TEST_FUNC(name) static void TEST_FUNC_NAME(name) (LFramework::Testing::TestResult& result)

#define TEST_OBJECT_NAME(name) test_##name##Object

#define TEST_REGISTER(name) \
	LFramework::Testing::Test TEST_OBJECT_NAME(name) (#name, TEST_FUNC_NAME(name) ); \

#define TEST_IMPORT(name) extern LFramework::Testing::Test TEST_OBJECT_NAME(name);

#define TEST(name) \
	TEST_FUNC(name); \
	TEST_REGISTER(name); \
	TEST_FUNC(name)

#define TEST_ASSERT(__expression, __message)\
	if(!(__expression)) { \
		result.message =  __message; \
		result.success = false; \
		return; \
	}

#define TEST_TRUE(__expression) \
		TEST_ASSERT(__expression, "Expression is not true: " #__expression)


#define TEST_RUN(name) runTest(&TEST_OBJECT_NAME(name))


	TestResult runTest(Test* test);

}}
