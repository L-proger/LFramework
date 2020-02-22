#pragma once

#include "LFrameworkConfig.h"

//OS Types
#define LF_OS_NONE 0
#define LF_OS_WINDOWS 1
#define LF_OS_LINUX 2
#define LF_OS_ANDROID 3
#define LF_OS_FREERTOS 4

//BSD family
#define LF_OS_BSD_DRAGONFLY 5
#define LF_OS_BSD_FREE 6
#define LF_OS_BSD_NET 7
#define LF_OS_BSD_OPEN 8
#define LF_OS_BSD_BSDI 9

//Apple
#define LF_OS_IPHONE 10
#define LF_OS_MAC 11

#define LF_OS_CYGWIN 12
#define LF_OS_LYNX 13
#define LF_OS_MSDOS 14
#define LF_OS_NUCLEUS_RTOS 15
#define LF_OS_OS2 16
#define LF_OS_PALMOS 17
#define LF_OS_SOLARIS 18


//OS Detect
#if !defined(LF_TARGET_OS)
#if defined(_WIN32) || defined(_WIN16) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
	#define LF_TARGET_OS LF_OS_WINDOWS
#elif defined(__linux__)
	#define LF_TARGET_OS LF_OS_LINUX
#elif defined(__ANDROID__)
	#define LF_TARGET_OS LF_OS_ANDROID

//Detect BDS
#elif defined(__DragonFly__)
	#define LF_TARGET_OS LF_OS_BSD_DRAGONFLY
#elif defined(__FreeBSD__)
	#define LF_TARGET_OS LF_OS_BSD_FREE
#elif defined(__NetBSD__)
	#define LF_TARGET_OS LF_OS_BSD_NET
#elif defined(__OpenBSD__)
	#define LF_TARGET_OS LF_OS_BSD_OPEN
#elif defined(__bsdi__)
	#define LF_TARGET_OS LF_OS_BSD_BSDI

//Detect Apple
#elif defined(__APPLE__) && defined(__MACH__)
	#include <TargetConditionals.h>
	#if (TARGET_IPHONE_SIMULATOR == 1) || (TARGET_OS_IPHONE == 1)
		#define LF_TARGET_OS LF_OS_IPHONE
	#elif TARGET_OS_MAC == 1
		#define LF_TARGET_OS LF_OS_MAC
	#else
		#error "Unknown Apple OS"
	#endif

#elif defined(__CYGWIN__)
#define LF_TARGET_OS LF_OS_CYGWIN
#elif defined(__Lynx__)
#define LF_TARGET_OS LF_OS_LYNX


//detect MS-DOS
#elif defined(MSDOS) || defined(__MSDOS__) || defined(_MSDOS) || defined(__DOS__)
	#define LF_TARGET_OS LF_OS_MSDOS

#elif defined(__nucleus__)
#define LF_TARGET_OS LF_OS_NUCLEUS_RTOS

#elif defined(OS2) || defined(_OS2) || defined(__OS2__) || defined(__TOS_OS2__)
#define LF_TARGET_OS LF_OS_OS2

#elif defined(__palmos__)
#define LF_TARGET_OS LF_OS_PALMOS

#elif defined(sun) || defined(__sun) 
#define LF_TARGET_OS LF_OS_SOLARIS

#else
	#error "OS auto detect fail!"
#endif
#endif

namespace LFramework {

	enum class OSType {
		None = LF_OS_NONE,
		Windows = LF_OS_WINDOWS,
		Linux = LF_OS_LINUX,
		Android = LF_OS_ANDROID,
		FreeRTOS = LF_OS_FREERTOS,

		BSDDragonfly = LF_OS_BSD_DRAGONFLY,
		BSDFree = LF_OS_BSD_FREE,
		BSDNet = LF_OS_BSD_NET,
		BSDOpen = LF_OS_BSD_OPEN,
		BSDI = LF_OS_BSD_BSDI,

		AppleIPhone = LF_OS_IPHONE,
		AppleMacOSX = LF_OS_MAC,

		Cygwin = LF_OS_CYGWIN,
		Lynx = LF_OS_LYNX,
		MSDOS = LF_OS_MSDOS,
		NucleusRTOS = LF_OS_NUCLEUS_RTOS,
		OS2 = LF_OS_OS2,
		PlamOS = LF_OS_PALMOS,
		Solaris = LF_OS_SOLARIS
	};

	namespace Detect {
		static constexpr inline OSType operatingSystem() {
			return static_cast<OSType>(LF_TARGET_OS);
		}
	}
}
