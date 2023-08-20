#pragma once

#include "ComObject.h"

#if defined(__CYGWIN32__)
#define LFRAMEWORK_COM_LIBRARY_CALL __stdcall
#define LFRAMEWORK_COM_LIBRARY_EXPORT __declspec(dllexport)
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(WINAPI_FAMILY)
#define LFRAMEWORK_COM_LIBRARY_CALL __stdcall
#define LFRAMEWORK_COM_LIBRARY_EXPORT __declspec(dllexport)
#elif defined(__MACH__)
#define LFRAMEWORK_COM_LIBRARY_CALL
#define LFRAMEWORK_COM_LIBRARY_EXPORT
#elif defined(__linux__) || defined(__ANDROID__)
#define LFRAMEWORK_COM_LIBRARY_CALL
#define LFRAMEWORK_COM_LIBRARY_EXPORT __attribute__ ((visibility ("default")))
#else
#define LFRAMEWORK_COM_LIBRARY_CALL
#define LFRAMEWORK_COM_LIBRARY_EXPORT
#endif

namespace LFramework {

#define LFRAMEWORK_COM_LIBRARY_EXPORT_ENTRY_FUNCTION() \
    extern "C" LFRAMEWORK_COM_LIBRARY_EXPORT void LFRAMEWORK_COM_LIBRARY_CALL getInterface(LFramework::ComPtr<LFramework::IUnknown>& result){ \
        result = LFramework::ComPtr<ILibrary>::create<Library>(); \
    }


}