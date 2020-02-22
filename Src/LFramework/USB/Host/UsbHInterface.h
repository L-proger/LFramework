#pragma once

#include "../UsbInterface.h"
#include <memory>
#include <vector>
#include <winusb.h>
#include <Windows.h>
#include "../UsbException.h"
#include "UsbHEndpoint.h"

namespace LFramework::USB {

class UsbHInterface : public UsbInterface {
public:
    UsbHInterface(const USB::InterfaceDescriptor& descriptor, HANDLE device, WINUSB_INTERFACE_HANDLE handle):_descriptor(descriptor), _handle(handle){


        for (UCHAR epId = 0; epId < descriptor.bNumEndpoints; epId++) {
            WINUSB_PIPE_INFORMATION pipe{};
            if(WinUsb_QueryPipe(_handle, descriptor.bAlternateSetting, epId, &pipe) == FALSE){
                throw UsbException("Failed to query endpoint descriptor");
            }

            USB::EndpointDescriptor endpointDescriptor;
            endpointDescriptor.wMaxPacketSize = pipe.MaximumPacketSize;
            endpointDescriptor.bInterval = pipe.Interval;
            endpointDescriptor.bEndpointAddress = pipe.PipeId;
            endpointDescriptor.bmAttributes.setType(static_cast<EndpointType>(pipe.PipeType));

            if(endpointDescriptor.bEndpointAddress.isIn()){
                _inEndpoints.push_back(std::make_shared<UsbHEndpoint>(endpointDescriptor, device, handle));
            }else{
                _outEndpoints.push_back(std::make_shared<UsbHEndpoint>(endpointDescriptor, device, handle));
            }
        }
    }

    UsbHEndpoint* getEndpoint(bool isInEndpoint, uint8_t id) override {
        return const_cast<UsbHEndpoint*>(getEndpointImpl(isInEndpoint, id));
    }
    const UsbHEndpoint* getEndpoint(bool isInEndpoint, uint8_t id) const override {
        return getEndpointImpl(isInEndpoint, id);
    }
    const USB::InterfaceDescriptor& getInterfaceDescriptor() const override {
        return _descriptor;
    }
private:
    const UsbHEndpoint* getEndpointImpl(bool isInEndpoint, uint8_t id) const {
        auto& endpoints = isInEndpoint ? _inEndpoints : _outEndpoints;
        if(id >= endpoints.size()){
            return nullptr;
        }
        return endpoints[id].get();
    }
    USB::InterfaceDescriptor _descriptor;
    WINUSB_INTERFACE_HANDLE _handle;

    std::vector<std::shared_ptr<UsbHEndpoint>> _inEndpoints;
    std::vector<std::shared_ptr<UsbHEndpoint>> _outEndpoints;
};

}

