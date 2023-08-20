#pragma once

#include <string>
#include <Windows.h>
#include <winusb.h>
#include <memory>
#include <vector>
#include "UsbHostEndpoint.h"
#include "UsbHInterface.h"
#include <LFramework/USB/Host/IUsbDevice.h>

namespace LFramework::USB {

class UsbHDevice : public IUsbDevice {
public:

    UsbHDevice(const std::string& deviceFilePath);
    std::shared_ptr<IUsbInterface> getInterface(size_t id) {
        if(id >= _interfaces.size()){
            return nullptr;
        }
        return _interfaces[id];
    }

    std::size_t getInterfaceCount() const {
        return _interfaces.size();
    }

private:
    HANDLE _deviceHandle;
    WINUSB_INTERFACE_HANDLE _interfaceHandle = nullptr;

    void enumerateInterfaces() {
        auto result = true;

        if(WinUsb_Initialize(_deviceHandle, &_interfaceHandle) == FALSE){
            throw UsbException("WinUsb_Initialize failed");
        }

        USB_INTERFACE_DESCRIPTOR interfaceDescriptor{};
        UCHAR alternateSetting = 0;

        //TODO: use  WinUsb_GetAssociatedInterface to enumerate ALL interfaces

        if(WinUsb_QueryInterfaceSettings(_interfaceHandle, alternateSetting, &interfaceDescriptor) == FALSE){
            throw UsbException("Failed to get interface descriptor");
        }

        USB::InterfaceDescriptor descriptor{};
        descriptor.iInterface = interfaceDescriptor.iInterface;
        descriptor.bNumEndpoints = interfaceDescriptor.bNumEndpoints;
        descriptor.bInterfaceNumber = interfaceDescriptor.bInterfaceNumber;
        descriptor.bAlternateSetting = interfaceDescriptor.bAlternateSetting;
        descriptor.classDescription.classId = interfaceDescriptor.bInterfaceClass;
        descriptor.classDescription.subClassId = interfaceDescriptor.bInterfaceSubClass;
        descriptor.classDescription.protocol = interfaceDescriptor.bInterfaceProtocol;

        _interfaces.push_back(std::make_shared<UsbHInterface>(descriptor, _deviceHandle, _interfaceHandle));

       /*if (querySettingsResult) {
           interfaceParameters->interfaceSubClass = interfaceDescriptor.bInterfaceSubClass;
           interfaceParameters->interfaceClass = interfaceDescriptor.bInterfaceClass;
           for (UCHAR index = 0; index < interfaceDescriptor.bNumEndpoints; index++) {
               auto queryPipeResult = static_cast<bool>(WinUsb_QueryPipe(interfaceHandle, 0, index, &pipe));
               if (queryPipeResult ){
                   UsbEndpointParameters endpointParameters{};

                   endpointParameters.endpointType = castWinUsbEndpointType(pipe.PipeType);
                   endpointParameters.maxPacketSize = pipe.MaximumPacketSize;
                   endpointParameters.interval = pipe.Interval;
                   endpointParameters.address = pipe.PipeId;
                   if(USB_ENDPOINT_DIRECTION_IN(pipe.PipeId)){
                       interfaceParameters->inEndpoints.push_back(UsbEndpoint(endpointParameters));
                   }
                   else{
                       interfaceParameters->outEndpoints.push_back(UsbEndpoint(endpointParameters));
                   }
               }
           }
       }*/


        /*auto nextInterfaceHandle = interfaceHandle;
        UCHAR index = 0;
        auto getInterfaceResult = true;
        while(getInterfaceResult){
            UsbInterfaceParameters interfaceParameters;
            initInterface(nextInterfaceHandle, &interfaceParameters);
            interfaces.push_back(UsbInterface(interfaceParameters));
            interfaceHandles.push_back(nextInterfaceHandle);
            for (auto &outEndpoint : interfaceParameters.outEndpoints)
                endpointsMap.insert(std::make_pair(outEndpoint.getAddress(), index));
            for (auto &inEndpoint : interfaceParameters.inEndpoints)
                endpointsMap.insert(std::make_pair(inEndpoint.getAddress(), index));
            getInterfaceResult = static_cast<bool>(WinUsb_GetAssociatedInterface(interfaceHandle, ++index, &nextInterfaceHandle));
        }*/


    }

private:
    std::vector<std::shared_ptr<UsbHInterface>> _interfaces;
};

}

