#include "TerminalStream.h"

#include <LFrameworkConfig.h>

#ifdef LF_TERMINAL_STREAM_COUT

#include <iostream>

namespace LFramework::Terminal {
    int LFramework::Terminal::Stream::write(const char* data, int length) {
        std::cout.write(data, length);
        std::cout.flush();
        return length;
    }
}

#endif
