#pragma once

#include "../IUsbInterface.h"
#include <stdexcept>
#include "SysfsUtils.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <sys/types.h>
#include <dirent.h>

namespace LFramework::USB {


EndpointType toEndpointType(const std::string &endpointTypeStr) {
    if(endpointTypeStr == "Bulk"){
        return EndpointType::Bulk;
    }else if(endpointTypeStr == "Interrupt"){
        return EndpointType::Interrupt;
    }else if(endpointTypeStr == "Control"){
        return EndpointType::Control;
    }else if(endpointTypeStr == "Isochronous"){
        return EndpointType::Isochronous;
    }else {
        throw std::runtime_error("Unknown endpoint type");
    }
}


class UsbInterface : public IUsbInterface {
public:
    UsbInterface(int deviceFileDescriptor, const std::string& deviceDirPath, const std::string& interfaceDirPath) 
        : _deviceFileDescriptor(deviceFileDescriptor), _deviceDirPath(deviceDirPath), _interfaceDirPath(interfaceDirPath){

        _descriptor.bAlternateSetting = SysfsUtils::readIntDeviceAttribute<std::uint8_t>(interfaceDirPath, "bAlternateSetting", 16);
        _descriptor.bInterfaceNumber = SysfsUtils::readIntDeviceAttribute<std::uint8_t>(interfaceDirPath, "bInterfaceNumber", 16);
        _descriptor.bNumEndpoints = SysfsUtils::readIntDeviceAttribute<std::uint8_t>(interfaceDirPath, "bNumEndpoints", 16);
        _descriptor.classDescription.classId = SysfsUtils::readIntDeviceAttribute<std::uint8_t>(interfaceDirPath, "bInterfaceClass", 16);
        _descriptor.classDescription.protocol = SysfsUtils::readIntDeviceAttribute<std::uint8_t>(interfaceDirPath, "bInterfaceProtocol", 16);
        _descriptor.classDescription.subClassId = SysfsUtils::readIntDeviceAttribute<std::uint8_t>(interfaceDirPath, "bInterfaceSubClass", 16);

        //TODO: implement somehow
        //_descriptor.iInterface


        enumerateEndpoints();
    }

    IUsbEndpoint* getEndpoint(bool isInEndpoint, uint8_t id) override{
        throw std::runtime_error("Not implemented");
    }
    const IUsbEndpoint* getEndpoint(bool isInEndpoint, uint8_t id) const override{
        throw std::runtime_error("Not implemented");
    }
    const USB::InterfaceDescriptor& getInterfaceDescriptor() const override{
        return _descriptor;
    }
private:
    USB::InterfaceDescriptor _descriptor;
    int _deviceFileDescriptor;
    std::string _deviceDirPath;
    std::string _interfaceDirPath;

    void enumerateEndpoints() {

        dirent *dirEntry;
        DIR *directory = nullptr;
        directory = opendir(_interfaceDirPath.c_str());
        if(directory == nullptr) {
            return;
        }
        while ((dirEntry = readdir(directory)) != nullptr) {
            if (dirEntry->d_type != DT_DIR) {
                continue;
            }
            if ((dirEntry->d_name[0] != 'e') || (dirEntry->d_name[1] != 'p') || (dirEntry->d_name[2] != '_')) {
                continue;
            }
           
            std::string endpointDirPath = _interfaceDirPath + dirEntry->d_name + "/";

            USB::EndpointDescriptor endpointDescriptor;
            endpointDescriptor.wMaxPacketSize = SysfsUtils::readIntDeviceAttribute<std::uint16_t>(endpointDirPath, "wMaxPacketSize", 16);
            endpointDescriptor.bInterval = SysfsUtils::readIntDeviceAttribute<std::uint8_t>(endpointDirPath, "bInterval", 16);
            endpointDescriptor.bEndpointAddress = SysfsUtils::readIntDeviceAttribute<std::uint8_t>(endpointDirPath, "bEndpointAddress", 16);
            endpointDescriptor.bmAttributes.setType(toEndpointType(SysfsUtils::readDeviceAttribute(endpointDirPath, "type")));

            if(endpointDescriptor.bEndpointAddress.isIn()){
                throw std::runtime_error("Not implemented");
                //_inEndpoints.push_back(std::make_shared<UsbHEndpoint>(endpointDescriptor, device, handle));
            }else{
                throw std::runtime_error("Not implemented");
                //_outEndpoints.push_back(std::make_shared<UsbHEndpoint>(endpointDescriptor, device, handle));
            }
        }
        closedir(directory);
        

    }
};

}