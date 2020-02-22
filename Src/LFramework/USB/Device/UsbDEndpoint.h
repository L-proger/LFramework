#pragma once

#include <cstdint>
#include <usbd_core.h>
#include <LFramework/USB/USBTypes.h>
#include "../UsbEndpoint.h"

namespace LFramework::USB {

class UsbDTransfer {
public:
	typedef void(*TransferCompleteCallback)(UsbDTransfer*, bool);
	void* buffer = nullptr;
	uint32_t size = 0;
	uint32_t actualSize = 0;
	void* userData = nullptr;
	TransferCompleteCallback callbackIsr = nullptr;
};

class UsbDEndpoint : public UsbEndpoint {
public:
	UsbDEndpoint(LFramework::USB::EndpointDescriptor descriptor):UsbEndpoint(descriptor){

	}

	bool open(USBD_HandleTypeDef *pdev){
		_isOpen = USBD_OK == USBD_LL_OpenEP(pdev, getDescriptor().bEndpointAddress.value, (uint8_t)getDescriptor().bmAttributes.getType(), getDescriptor().wMaxPacketSize);

		//FIX для L0 ядра и подобных. Зачем-то в них эндпоинты запускаются не с NAK статусом
#if defined(STM32F0)
		PCD_HandleTypeDef* hpcd = (PCD_HandleTypeDef*)pdev->pData;
		if(isInDirection()){ //TX EP
				PCD_SET_EP_TX_STATUS(hpcd->Instance, id, USB_EP_TX_NAK);
		}else{
				PCD_SET_EP_RX_STATUS(hpcd->Instance, id, USB_EP_RX_NAK);
		}
#endif
		return _isOpen;
	}

	void close(USBD_HandleTypeDef *pdev){
		USBD_LL_CloseEP(pdev, getDescriptor().bEndpointAddress.value);
		_isOpen = false;
		//finalizeTransfer(false, 0);
	}
	bool transferAsync(UsbDTransfer* transfer);

	bool isOpen() const {
		return _isOpen;
	}
	void onTxCompleteIsr();
	void onRxCompleteIsr(uint32_t rxLength);
protected:
	void finalizeTransfer(bool success, uint32_t size){
		if(_activeTransfer != nullptr){
			auto transfer = _activeTransfer;
			_activeTransfer = nullptr;
			transfer->actualSize = size;
			if(transfer->callbackIsr != nullptr){
				transfer->callbackIsr(transfer, success);
			}
		}
	}
private:
	UsbDTransfer* _activeTransfer = nullptr;

	bool _isOpen = false;
};
}
