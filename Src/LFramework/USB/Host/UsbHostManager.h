#pragma once

#include <vector>
#include <string>
#include <Windows.h>
#include <SetupAPI.h>
#include <stdexcept>
#include <regex>

struct UsbHostDeviceInfo {
    uint16_t vid;
    uint16_t pid;
    std::string serialNumber;
    std::string path;
};

class UsbHostManager {
public:

    static UsbHostDeviceInfo getUsbDeviceInfo(const std::string& path){
        UsbHostDeviceInfo result{};
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

    static std::vector<UsbHostDeviceInfo> enumerateDevices(){
        std::vector<UsbHostDeviceInfo> result;
        GUID usbClassGUID = { 0xA5DCBF10, 0x6530, 0x11D2, {0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED} };

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
            deviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

            SP_DEVINFO_DATA deviceData{};
            deviceData.cbSize = sizeof(deviceData);

            if (SetupDiGetDeviceInterfaceDetailA(deviceList, &deviceInterfaceData, deviceInterfaceDetailData, deviceInterfaceDataSize, &deviceInterfaceDataSize, &deviceData) == TRUE){
                auto info = getUsbDeviceInfo(std::string(deviceInterfaceDetailData->DevicePath));
                result.push_back(info);
            }
        }
        SetupDiDestroyDeviceInfoList(deviceList);
        return result;
    }
};
