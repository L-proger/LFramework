
#include <LFrameworkConfig.h>
#ifdef LF_USB_DEVICE
#include <usbd_def.h>
#include <LFramework/USB/Device/UsbDDevice.h>

#include <LFramework/USB/Device/UsbDDevice.h>
#include <LFramework/USB/USBTypes.h>
#include <LFramework/USB/UsbMicrosoftTypes.h>

using namespace LFramework;

volatile USB::Microsoft::CompatId::WinUSBFunction _winusbFunctionDescriptor(0);


extern "C" USBD_StatusTypeDef USBD_hook_device_request(USBD_HandleTypeDef * pdev, USBD_SetupReqTypedef * req) {
	if ((req->bmRequest & USB_REQ_TYPE_MASK) == USB_REQ_TYPE_VENDOR) {
		if (req->bRequest == (uint8_t)USB_VENDOR_CODE_WINUSB) {
			if (req->wIndex == 0x04) {
				USBD_CtlSendData(pdev, (uint8_t*)&_winusbFunctionDescriptor, req->wLength);
				return USBD_OK;
			}
		}
	}
	return USBD_FAIL;
}
extern "C" USBD_StatusTypeDef USBD_hook_interface_request(USBD_HandleTypeDef * pdev, USBD_SetupReqTypedef * req) {
	return USB::UsbDevice::interfaceRequest(pdev, req);
}


extern "C" void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef * hpcd) {
	USBD_HandleTypeDef* pdev = (USBD_HandleTypeDef*)hpcd->pData;

	//Hack to support WinUSB requests :(
	USBD_ParseSetupRequest(&pdev->request, (uint8_t*)hpcd->Setup);
	pdev->ep0_state = USBD_EP0_SETUP;
	pdev->ep0_data_len = pdev->request.wLength;

	switch (pdev->request.bmRequest & 0x1F) {
	case USB_REQ_RECIPIENT_DEVICE:
		if (USBD_hook_device_request(pdev, &pdev->request) == USBD_OK) {
			return;
		}
		break;
	case USB_REQ_RECIPIENT_INTERFACE:
		if (USBD_hook_interface_request(pdev, &pdev->request) == USBD_OK) {
			return;
		}
		break;
	}

	USBD_LL_SetupStage((USBD_HandleTypeDef*)hpcd->pData, (uint8_t*)hpcd->Setup);
}

extern "C" void HAL_PCD_SOFCallback2(PCD_HandleTypeDef * hpcd) {
	while (true) {
		//asm("nop");
	}
}



extern "C" void HAL_PCD_ResetCallback(PCD_HandleTypeDef * hpcd) {
	USBD_SpeedTypeDef speed = USBD_SPEED_FULL;
	switch (hpcd->Init.speed) {
	case PCD_SPEED_FULL:
		speed = USBD_SPEED_FULL;
		break;
	default:
		speed = USBD_SPEED_FULL;
		break;
	}
	USBD_LL_SetSpeed((USBD_HandleTypeDef*)hpcd->pData, speed);
	USBD_LL_Reset((USBD_HandleTypeDef*)hpcd->pData);
}

extern "C" void HAL_PCD_SuspendCallback(PCD_HandleTypeDef * hpcd) {
	//Inform USB library that core enters in suspend Mode
	if (hpcd->pData != 0) {
		USBD_LL_Suspend((USBD_HandleTypeDef*)hpcd->pData);
	}

	//Enter in STOP mode
	if (hpcd->Init.low_power_enable) {
		//Set SLEEPDEEP bit and SleepOnExit of Cortex System Control Register
		SCB->SCR |= (uint32_t)((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
	}
}

extern "C" void HAL_PCD_ResumeCallback(PCD_HandleTypeDef * hpcd) {
	while (true) {
		asm("nop");
	}
}

extern "C" void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef * hpcd, uint8_t epnum) {
	while (true) {
		asm("nop");
	}
}

extern "C" void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef * hpcd, uint8_t epnum) {
	while (true) {
		asm("nop");
	}
}

extern "C" void HAL_PCD_ConnectCallback(PCD_HandleTypeDef * hpcd) {
	while (true) {
		asm("nop");
	}
}

extern "C" void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef * hpcd) {
	while (true) {
		asm("nop");
	}
}

#endif