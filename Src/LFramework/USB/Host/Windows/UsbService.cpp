#include <LFrameworkConfig.h>
#include "UsbService.h"

#include <initguid.h>
#include <Usbiodef.h>
#include <Windows.h>
#include <SetupAPI.h>
#include <stdexcept>
#include <regex>
#include <future>

#include "UsbNotifyWindow.h"


namespace LFramework::USB {

static std::thread _eventsPollingThread;

UsbDeviceInfo UsbService::getUsbDeviceInfo(const std::string& path){
    UsbDeviceInfo result{};
    std::smatch match;
    std::regex vidPidRegex(".*vid_([0-9A-Fa-f]{4})&pid_([0-9A-Fa-f]{4}).*");
    std::regex serialNumberRegex(".*#([^#]*)#.*");
    if (std::regex_match(path, match, serialNumberRegex))
        result.serialNumber = match[1].str();
    if (std::regex_match(path, match, vidPidRegex)) {
        const uint8_t base = 16;
        result.vid = static_cast<uint16_t>(std::stoi(match[1].str(), nullptr, base));
        result.pid = static_cast<uint16_t>(std::stoi(match[2].str(), nullptr, base));
    }
    result.path = path;
    return result;
}

std::vector<UsbDeviceInfo> UsbService::enumerateDevices(){
    std::vector<UsbDeviceInfo> result;
    GUID usbClassGUID = GUID_DEVINTERFACE_USB_DEVICE;

    //Get device list
    auto deviceList = SetupDiGetClassDevsA(&usbClassGUID, nullptr, nullptr, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
    if (deviceList == INVALID_HANDLE_VALUE) {
        return result;
    }

    //Iterate devices
    for(DWORD i = 0; GetLastError() != ERROR_NO_MORE_ITEMS; ++i){
        //Get device interface
        SP_DEVICE_INTERFACE_DATA deviceInterfaceData{};
        deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
        if(SetupDiEnumDeviceInterfaces(deviceList, nullptr, &usbClassGUID, i, &deviceInterfaceData) == FALSE){
            if( GetLastError() != ERROR_NO_MORE_ITEMS){
                continue;
            }else{
                break;
            }
        }

        //Get device interface details
        DWORD deviceInterfaceDataSize = 0;
        SetupDiGetDeviceInterfaceDetailA(deviceList, &deviceInterfaceData, nullptr, 0, &deviceInterfaceDataSize, nullptr);

        std::vector<std::uint8_t> deviceInterfaceDetailBuffer;
        deviceInterfaceDetailBuffer.resize(deviceInterfaceDataSize, 0);

        auto deviceInterfaceDetailData = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA_A>(deviceInterfaceDetailBuffer.data());
        deviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);

        SP_DEVINFO_DATA deviceData{};
        deviceData.cbSize = sizeof(deviceData);

        if (SetupDiGetDeviceInterfaceDetailA(deviceList, &deviceInterfaceData, deviceInterfaceDetailData, deviceInterfaceDataSize, &deviceInterfaceDataSize, &deviceData) == TRUE){

            auto info = getUsbDeviceInfo(std::string(deviceInterfaceDetailData->DevicePath));
            result.push_back(info);
        } else {
            auto err = GetLastError();
        }
    }
    SetupDiDestroyDeviceInfoList(deviceList);
    return result;
}


DWORD WINAPI UsbService::eventsThreadProc(LPVOID lpParam){
    UsbService* _this = reinterpret_cast<UsbService*>(lpParam);

    std::shared_ptr<UsbNotifyWindow> window;
    try{
        window = std::make_shared<UsbNotifyWindow>(_this->_deviceChangeCallback);
        _this->_initPromise.set_value(true);
    }catch(...){
        _this->_initPromise.set_value(false);
        return 0;
    }
    window->processMessages();
    window = nullptr;
    return 0;
}

bool UsbService::startEventsListening(std::function<void()> deviceChangeCallback){
    if(_threadHandle != nullptr){
        return false;
    }

    _initPromise = std::promise<bool>();
    auto initFuture = _initPromise.get_future();

    _deviceChangeCallback = deviceChangeCallback;

    _threadHandle = CreateThread(nullptr, 0, &UsbService::eventsThreadProc, this, 0, &_threadId);

    if(!initFuture.get()){
        WaitForSingleObject(_threadHandle, INFINITE);
        CloseHandle(_threadHandle);
        _threadHandle = nullptr;
        _deviceChangeCallback = nullptr;
        return false;
    }
    return true;
}

void UsbService::stopEventsListening(){
    if(_threadHandle == nullptr){
        return;
    }

    while(PostThreadMessageA(_threadId, WM_QUIT, 0, 0) != TRUE){
        std::this_thread::yield();
    }

    WaitForSingleObject(_threadHandle, INFINITE);
    CloseHandle(_threadHandle);
    _threadHandle = nullptr;
    _deviceChangeCallback = nullptr;
}


IUsbService* createUsbService(){
    return new UsbService();
}


}
