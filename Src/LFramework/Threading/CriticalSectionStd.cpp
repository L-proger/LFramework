#include <LFramework/Detect/DetectOS.h>

#if (LF_TARGET_OS == LF_OS_WINDOWS) || (LF_TARGET_OS == LF_OS_LINUX)
#include <LFramework/Threading/CriticalSectionStd.h>

std::recursive_mutex LFramework::Threading::CriticalSection::_lock;
#endif
