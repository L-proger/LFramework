#ifdef _WIN32

#include "UsbHDevice.h"
#include "../UsbException.h"
#include "../USBTypes.h"

namespace LFramework::USB {

UsbHDevice::UsbHDevice(const std::string& deviceFilePath){
    _deviceHandle = CreateFileA(deviceFilePath.c_str(),
                      GENERIC_WRITE | GENERIC_READ,
                      FILE_SHARE_WRITE | FILE_SHARE_READ,
                      nullptr,
                      OPEN_EXISTING,
                      FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                      nullptr);

    if(_deviceHandle == INVALID_HANDLE_VALUE){
         throw UsbException("Failed top open device file");
    }

    enumerateInterfaces();
}

}

#endif
