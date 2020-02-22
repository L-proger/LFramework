#pragma once

#include "USBTypes.h"
#include "UsbEndpoint.h"

#define USB_VENDOR_CODE_WINUSB 'P'

namespace LFramework::USB {

class UsbInterface {
public:
    virtual ~UsbInterface() = default;
    virtual UsbEndpoint* getEndpoint(uint8_t address) {
    	bool isIn = USB::EndpointAddress::isIn(address);
    	uint8_t id = 0;
    	while(true){
    		auto ep = getEndpoint(isIn, id++);
    		if(ep == nullptr){
    			return nullptr;
    		}
    		if(ep->getDescriptor().bEndpointAddress.value == address){
    			return ep;
    		}
    	}
    }
    virtual UsbEndpoint* getEndpoint(bool isInEndpoint, uint8_t id) = 0;
    virtual const UsbEndpoint* getEndpoint(bool isInEndpoint, uint8_t id) const = 0;
    virtual const USB::InterfaceDescriptor& getInterfaceDescriptor() const = 0;
};

}
