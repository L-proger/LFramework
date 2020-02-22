#pragma once
#include <LFrameworkConfig.h>

#ifdef LF_USB_DEVICE
#include "../UsbInterface.h"

#include "usbd_def.h"
#include <LFramework/USB/USBTypes.h>
#include <LFramework/USB/UsbMicrosoftTypes.h>
#include <cstddef>
#include <cstdint>

#include "UsbDEndpoint.h"
#include "../UsbInterface.h"
#define USB_VENDOR_CODE_WINUSB 'P'


namespace LFramework::USB {

class UsbDInterface : public UsbInterface {
public:
    virtual bool init(USBD_HandleTypeDef* pdev) {


        for(uint32_t i = 0;; ++i){
            auto ep = getEndpoint(false, i);
            if(ep == nullptr){
            	break;
            }
            static_cast<UsbDEndpoint*>(ep)->open(pdev);
        }

        for(uint32_t i = 0;; ++i){
			auto ep = getEndpoint(true, i);
			if(ep == nullptr){
				break;
			}
			static_cast<UsbDEndpoint*>(ep)->open(pdev);
		}

        _isOpen = true;
        return true;
    }
    virtual bool deinit(USBD_HandleTypeDef* pdev) {
    	for(uint32_t i = 0;; ++i){
			auto ep = getEndpoint(false, i);
			if(ep == nullptr){
				break;
			}
			static_cast<UsbDEndpoint*>(ep)->close(pdev);
		}

		for(uint32_t i = 0;; ++i){
			auto ep = getEndpoint(true, i);
			if(ep == nullptr){
				break;
			}
			static_cast<UsbDEndpoint*>(ep)->close(pdev);
		}
        _isOpen = false;
        return true;
    }

    uint32_t getDescriptor(uint8_t* buffer, uint32_t bufferSize, uint32_t interface_id) const {
    	*reinterpret_cast<LFramework::USB::InterfaceDescriptor*>(buffer) = getInterfaceDescriptor();
    	buffer += sizeof(LFramework::USB::InterfaceDescriptor);

        for(uint8_t i = 0;; ++i){
        	auto ep = getEndpoint(false, i);
        	if(ep == nullptr){
        		break;
        	}
            makeEndpointDescriptor(buffer, ep);
            buffer += sizeof(LFramework::USB::EndpointDescriptor);
        }

        for(uint8_t i = 0;; ++i){
			auto ep = getEndpoint(true, i);
			if(ep == nullptr){
				break;
			}
			makeEndpointDescriptor(buffer, ep);
			buffer += sizeof(LFramework::USB::EndpointDescriptor);
		}

        return sizeof(LFramework::USB::InterfaceDescriptor) + getInterfaceDescriptor().bNumEndpoints * sizeof(LFramework::USB::EndpointDescriptor);
    }

    Microsoft::Property::InterfaceGuid _interfaceGuidDescriptor;

    USBD_StatusTypeDef interfaceRequest(USBD_HandleTypeDef* pdev, USBD_SetupReqTypedef* req) {
        if((req->bmRequest & USB_REQ_TYPE_MASK) == USB_REQ_TYPE_VENDOR){
            if(req->bRequest == (uint8_t)USB_VENDOR_CODE_WINUSB){
                if (req->wIndex == 0x05){
                    if(req->wLength != 10){
                        USBD_CtlSendData (pdev, (uint8_t*)&_interfaceGuidDescriptor, req->wLength);
                    }else {
                        USBD_CtlSendData (pdev, (uint8_t*)&_interfaceGuidDescriptor, req->wLength);
                    }
                    return USBD_OK;
                }else{
                    USBD_CtlError(pdev, req);
                    return USBD_OK;
                }
            }
        }
        return USBD_FAIL;
    }

    static void makeEndpointDescriptor(uint8_t* buffer, const UsbEndpoint* ep){
    	*reinterpret_cast<LFramework::USB::EndpointDescriptor*>(buffer) = ep->getDescriptor();
    }

	virtual void txComplete(uint8_t epnum) {
		auto ep = getEndpoint(USB::EndpointAddress::makeIn(epnum));
		if(ep != nullptr){
			static_cast<UsbDEndpoint*>(ep)->onTxCompleteIsr();
		}
	}
	virtual void rxComplete(uint32_t rx_length, uint8_t epnum) {
		auto ep = getEndpoint(USB::EndpointAddress::makeOut(epnum));
		if(ep != nullptr){
			static_cast<UsbDEndpoint*>(ep)->onRxCompleteIsr(rx_length);
		}
	}
	bool isOpen() const {
		return _isOpen;
	}
private:
    bool _isOpen = false;
};

}

#endif
