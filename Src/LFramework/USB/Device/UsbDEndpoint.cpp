#include <LFrameworkConfig.h>

#ifdef LF_USB_DEVICE

#include "UsbDEndpoint.h"
#include "UsbDDevice.h"

namespace LFramework::USB {

bool UsbDEndpoint::transferAsync(UsbTransfer* transfer){
	//CriticalSectionLock lock;

	if(transfer == nullptr || _activeTransfer != nullptr || !isOpen()){
		return false;
	}

	_activeTransfer = transfer;

	if(getDescriptor().bEndpointAddress.isIn()){
		if(USBD_OK != USBD_LL_Transmit(&UsbDevice::hUsbDevice, getDescriptor().bEndpointAddress.value, (uint8_t*)transfer->buffer, transfer->size)){
			_activeTransfer = nullptr;
			return false;
		}
	}else{
		if(USBD_OK != USBD_LL_PrepareReceive(&UsbDevice::hUsbDevice, getDescriptor().bEndpointAddress.value, (uint8_t*)transfer->buffer, transfer->size)){
			_activeTransfer = nullptr;
			return false;
		}
	}
	return true;
}

void UsbDEndpoint::onTxCompleteIsr(){
	finalizeTransfer(true, 0);
}

void UsbDEndpoint::onRxCompleteIsr(uint32_t rxLength){
	finalizeTransfer(true, rxLength);
}

}

#endif
