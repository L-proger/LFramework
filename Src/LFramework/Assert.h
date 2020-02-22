#pragma once

#include "Debug.h"
#include "IO/Terminal/Terminal.h"

namespace LFramework::Debug {
    class Assert : public LogWriter {
	public:
        Assert():LogWriter(LogLevel::Fatal){}
		~Assert(){
			for(;;);
		}
	};


#define lfAssert(expression) \
	do { \
		if(!(expression)) { \
			LFramework::Debug::Assert() \
				<< "Assert: " <<  #expression << LFramework::Terminal::NewLine \
				<< "File: " << __FILE__ << LFramework::Terminal::NewLine; \
		} \
	}while(false)

}

