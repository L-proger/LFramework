/*
 * UnitTest.cpp
 *
 *  Created on: 27 Aug. 2017 г.
 *      Author: l-pro
 */

#include "UnitTest.h"
#include "../Debug.h"

namespace LFramework { namespace Testing {

	LinkedList& getTestsList() {
		static LinkedList list;
		return list;
	}

	void runAllTests() {
		getTestsList().foreach([](auto node) { runTest((Test*)node);});
	}

	TestResult runTest(Test* test){
		lfDebug() << "[ RUN    ]" << test->name;
		auto result = test->run();
		lfDebug() << (result.success ? "[ PASSED ]" : "[ FAILED ]") << test->name;
		if (result.message != nullptr) {
			lfDebug() << result.message;
		}
		return result;
	}

}}




