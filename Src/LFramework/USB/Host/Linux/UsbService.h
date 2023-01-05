#pragma once

#include "../IUsbService.h"
#include "NetlinkClient.h"
#include "NetlinkReader.h"
#include <memory>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <sys/types.h>
#include <dirent.h>

#include <stdexcept>
#include <algorithm>

#include <fstream>

#include "SysfsUtils.h"

namespace LFramework::USB {

class UsbService : public IUsbService {
public:
    UsbService(){
       
    }
    UsbDeviceInfo getUsbDeviceInfo(const std::string& path) override {

    }

    

    std::vector<UsbDeviceInfo> enumerateDevices() override {
        std::vector<UsbDeviceInfo> result;
        dirent *entry = nullptr;
        DIR *devices = opendir("/sys/bus/usb/devices/");
        if(devices == nullptr) {
            throw std::runtime_error("Failed to open sysfs devices directory");
        }
        while ((entry = readdir(devices)) != nullptr) {
            std::string deviceName = entry->d_name;

            
            if(!SysfsUtils::isUsbDeviceName(deviceName)) {
                continue;
            }
            std::string devicePath = "/sys/bus/usb/devices/" + deviceName + '/';

            std::string idVendor = SysfsUtils::readDeviceAttribute(devicePath, "idVendor");
            if(idVendor.empty()){
                continue;
            }
            std::string idProduct = SysfsUtils::readDeviceAttribute(devicePath, "idProduct");
            if(idProduct.empty()){
                continue;
            }

            std::string serial = SysfsUtils::readDeviceAttribute(devicePath, "serial");

            UsbDeviceInfo info = {};
            info.path = devicePath;
            info.vid = std::strtol(idVendor.c_str(), nullptr, 16);
            info.pid = std::strtol(idProduct.c_str(), nullptr, 16);
            info.serialNumber = serial;


            result.emplace_back(info);
        }
        closedir(devices);
        return result;
    }
    bool startEventsListening(std::function<void()> deviceChangeCallback) override {
        _deviceChangeCallback = deviceChangeCallback;

        auto netlinkClient = std::make_shared<LFramework::USB::NetlinkClient>();
        _netlink = std::make_shared<LFramework::USB::NetlinkReader>(netlinkClient, [this](const NetlinkReader::DeviceEvent& event){ onUsbEvent(event); });

    }
    void stopEventsListening() override {
        _netlink = nullptr;
        _deviceChangeCallback = nullptr;

    }

private:
    std::function<void()> _deviceChangeCallback;
    std::shared_ptr<LFramework::USB::NetlinkReader> _netlink;

    void onUsbEvent(const NetlinkReader::DeviceEvent& event){
        _deviceChangeCallback();
    }
};

}