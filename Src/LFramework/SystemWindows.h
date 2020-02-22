#ifndef SystemWindows_h__
#define SystemWindows_h__

#include <Windows.h>

namespace LFramework {
	namespace System {
		inline size_t getCurrentInterruptHandlerIndex() {
			return 0;
		}

		inline bool isHandlingInterrupt() {
			return false;
		}

		inline bool isDebuggerAttached() {
			return IsDebuggerPresent() == TRUE;
		}

		inline void setBreakpoint() {
			if (isDebuggerAttached()) {
				DebugBreak();
			}
		}
	}
}

#endif // SystemWindows_h__
