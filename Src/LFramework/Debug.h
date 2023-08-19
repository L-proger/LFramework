#pragma once
#include "IO/Terminal/Terminal.h"

namespace LFramework::Debug {

	enum class LogLevel {
		Debug,
		Info,
		Warning,
		Critical,
		Fatal
	};

	inline const char* toString(LogLevel category) {
		switch (category) {
		case LogLevel::Debug:
			return "Debug";
		case LogLevel::Info:
			return "Info";
		case LogLevel::Warning:
			return "Warning";
		case LogLevel::Critical:
			return "Critical";
		case LogLevel::Fatal:
			return "Fatal";
		}
        return "";
	}

	class LogWriter {
	public:
		LogWriter(LogLevel level){

		}
		template<typename T>
		LogWriter& operator << (const T& value) {
			//Terminal::out << value;
			return *this;
		}

		~LogWriter() {
			//Terminal::out << Terminal::NewLine;
		}
	};

    inline LogWriter Log() { return LogWriter(LogLevel::Debug); }
    inline LogWriter Info() { return LogWriter(LogLevel::Info); }
    inline LogWriter Warning() { return LogWriter(LogLevel::Warning); }
    inline LogWriter Critical() { return LogWriter(LogLevel::Critical); }
    inline LogWriter Fatal() { return LogWriter(LogLevel::Fatal); }


	#define lfDebug() LFramework::Debug::Log()
	#define lfInfo() LFramework::Debug::Info()
	#define lfWarning() LFramework::Debug::Warning()
	#define lfCritical() LFramework::Debug::Critical()
	#define lfFatal() LFramework::Debug::Fatal()

}
