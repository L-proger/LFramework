#ifndef _WIN32

#include <LFrameworkConfig.h>

#ifdef LF_USB_DEVICE

#include "UsbDevice.h"
#include "usbd_ioreq.h"
#include "usbd_ctlreq.h"
#include "usbd_def.h"
#include "usbd_core.h"
#include <stm32f7xx_hal.h>
#include <LFramework/USB/USBTypes.h>
#include <LFramework/USB/UsbMicrosoftTypes.h>
#include <type_traits>

using namespace LFramework;

extern "C" {
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;
}

namespace LFramework::USB {
USBD_HandleTypeDef UsbDevice::hUsbDevice {};
UsbDDeviceContext* _context = nullptr;


volatile USB::Microsoft::CompatId::WinUSBFunction _winusbFunctionDescriptor(0);

//For other-speed description
__ALIGN_BEGIN volatile USB::DeviceQualifierDescriptor USBD_NDC_DeviceQualifierDesc __ALIGN_END {
	UsbVersion(2),
	USB::UsbClass::Device::UseInterfaceClass(),
	64,
	1,
	0
};

#define USB_VENDOR_CODE_WINUSB 'P'

__ALIGN_BEGIN volatile USB::Microsoft::MicrosoftStringDescriptor NDC_StringDescriptor __ALIGN_END = {
	(uint8_t)USB_VENDOR_CODE_WINUSB
};


extern volatile USB::DeviceQualifierDescriptor USBD_NDC_DeviceQualifierDesc;
extern volatile USB::Microsoft::MicrosoftStringDescriptor NDC_StringDescriptor;

std::aligned_storage_t<USBD_MAX_STR_DESC_SIZ, 4> _descriptorsBuffer;


UsbDDeviceContext* UsbDevice::getContext(){
	return _context;
}

void hang(){
	while(true){
		asm("nop");
	}
}

const USBD_DescriptorsTypeDef UsbDevice::_descriptorsTable = {
	&UsbDevice::getDeviceDescriptor,
	&UsbDevice::getLangidStrDescriptor,
	&UsbDevice::getManufacturerStringDescriptor,
	&UsbDevice::getProductStringDescriptor,
	&UsbDevice::getSerialStringDescriptor,
	&UsbDevice::getConfigStringDescriptor,
	&UsbDevice::getInterfaceStringDescriptor
};

const USBD_ClassTypeDef UsbDevice::_usbClassBinding = {
	&UsbDevice::coreInit,
	&UsbDevice::coreDeinit,
	&UsbDevice::coreSetup,
	0, //USBD_NDC_EP0_TxReady,
	&UsbDevice::coreEp0RxReady,
	&UsbDevice::coreDataIn,
	&UsbDevice::coreDataOut,
	&UsbDevice::coreSof,
	&UsbDevice::coreIsoInIncomplete,
	&UsbDevice::coreIsoOutIncomplete,
	&UsbDevice::coreGetCfgDesc,
	&UsbDevice::coreGetCfgDesc,
	&UsbDevice::coreGetCfgDesc,
	&UsbDevice::coreGetDeviceQualifierDesc,
	&UsbDevice::coreGetUserStringDesc
};

uint8_t UsbDevice::coreInit(USBD_HandleTypeDef* pdev, uint8_t cfgidx){
	//TODO: use configuration id
	for(size_t i = 0; i < _context->getInterfaceCount(); ++i){
		auto interface = _context->getInterface(i);
		if(interface != nullptr){
			if(!interface->init(pdev)){
				return USBD_FAIL;
			}
		}
	}
	return USBD_OK;
}
uint8_t UsbDevice::coreDeinit(USBD_HandleTypeDef* pdev, uint8_t cfgidx){
	//TODO: use configuration id

	for(size_t i = 0; i < _context->getInterfaceCount(); ++i){
		auto interface = _context->getInterface(i);
		if(interface != nullptr){
			if(!interface->deinit(pdev)){
				//return USBD_FAIL;
			}
		}
	}
	return USBD_OK;
}
uint8_t UsbDevice::coreImplSetup(USBD_SetupReqTypedef request, void* data){
	switch ( request.bmRequest & USB_REQ_RECIPIENT_MASK ){
	case USB_REQ_RECIPIENT_INTERFACE:{
		if(_context != nullptr){
			auto* interface = _context->getInterface(request.wValue);
			if(interface != nullptr){
				hang();
				/*if(interface->control(&hUsbDevice, request.bRequest, (uint8_t*)data, request.wLength)){
					return USBD_OK;
				}*/
			}
		}
		break;
	}

	case USB_REQ_RECIPIENT_ENDPOINT:
		hang();
		/*if(_usb_device_context != nullptr){
			if(request.bRequest == USB_REQ_CLEAR_FEATURE){ //reset pipe is called at host side
				//do reset pipe
				if(_clearFeatureCallback != nullptr){
					_clearFeatureCallback(request.wIndex);
				}
			}
		}*/
		break;
	case USB_REQ_RECIPIENT_DEVICE:
	default:
		break;
	}
	return USBD_OK;
}
uint8_t UsbDevice::coreSetup(USBD_HandleTypeDef* pdev, USBD_SetupReqTypedef *req){
	hang();
	/*if (req->wLength){
		//Request with data stage{
		if((req->bmRequest & USB_REQ_DATA_PHASE_MASK) == USB_REQ_DATA_PHASE_DEVICE_TO_HOST){
			//device to host data stage => handler should send data
			return coreImplSetup(*req, 0);
		}else{ //host to device data stage! Can't execute now, read data first & execute later in Ep0Receive callback
			last_request = *req;
			USBD_CtlPrepareRx (pdev, (uint8_t*)&ep0Buffer[0], req->wLength);
		}
	} else {//No data stage => simple request => execute now
		return coreImplSetup(*req, 0);
	}*/
	return USBD_OK;
}
uint8_t  UsbDevice::coreEp0RxReady(USBD_HandleTypeDef* pdev){
	hang();
	//coreImplSetup(last_request, &ep0Buffer[0]); //data in stage complete => execute request
	//last_request.bRequest = 0xff;
	return USBD_OK;
}

UsbDInterface* UsbDevice::findInterfaceByEndpointAddress(uint8_t address){
	if(_context == nullptr){
		return nullptr;
	}
	auto if_cnt = _context->getInterfaceCount();
	for(uint32_t i = 0; i < if_cnt; ++i){
		auto interface = _context->getInterface(i);
		if(interface != nullptr){
			auto endpoint = interface->getEndpoint(address);
			if(endpoint != nullptr){
				return interface;
			}
		}
	}
	return nullptr;
}

uint8_t  UsbDevice::coreDataIn(USBD_HandleTypeDef* pdev, uint8_t epnum){
	auto interface = findInterfaceByEndpointAddress(USB::EndpointAddress::makeIn(epnum));//TODO: cleanup
	if(interface != nullptr){
		interface->txComplete(epnum | 0x80);
	}
    return USBD_OK;
}
uint8_t  UsbDevice::coreDataOut(USBD_HandleTypeDef* pdev, uint8_t epnum){

	uint32_t rxLen = USBD_LL_GetRxDataSize (pdev, epnum);
	auto interface = findInterfaceByEndpointAddress(USB::EndpointAddress::makeOut(epnum));
	if(interface != nullptr){
		interface->rxComplete(rxLen, epnum);
	}
	return USBD_OK;
}
uint8_t  UsbDevice::coreSof(USBD_HandleTypeDef* pdev){
	hang();
	return USBD_OK;
}

uint8_t  UsbDevice::coreIsoInIncomplete(USBD_HandleTypeDef* pdev, uint8_t epnum){
	hang();
	return USBD_OK;
}

uint8_t  UsbDevice::coreIsoOutIncomplete(USBD_HandleTypeDef* pdev, uint8_t epnum){
	hang();
	return USBD_OK;
}

uint8_t* UsbDevice::coreGetCfgDesc(uint16_t* length){
	auto* mem = reinterpret_cast<uint8_t*>(&_descriptorsBuffer);
	auto* buffer = mem;
	auto* cd = new(buffer)USB::ConfigurationDescriptor();
	cd->wTotalLength 		= 0;
	cd->bNumInterfaces 		= (uint8_t)_context->getInterfaceCount();
	cd->bConfigurationValue = 0x01;
	cd->iConfiguration 		= USBD_IDX_CONFIG_STR;
	cd->bmAttributes 		= USB::UsbAttributes().value;
	cd->bMaxPower 			= 500;
	buffer += sizeof(USB::ConfigurationDescriptor);

	for(uint32_t i = 0; i < _context->getInterfaceCount(); ++i){
		auto emptySize = (mem + sizeof(_descriptorsBuffer)) - buffer;
		auto size = _context->getInterface(i)->getDescriptor(buffer, emptySize, i);
		buffer += size;
		cd->wTotalLength += size;
	}
	cd->wTotalLength += sizeof(USB::ConfigurationDescriptor);

	*length = cd->wTotalLength;
	return reinterpret_cast<uint8_t*>(&_descriptorsBuffer);
}


uint8_t* UsbDevice::getDeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t* length){
	auto mem = reinterpret_cast<uint8_t*>(&_descriptorsBuffer);
	auto& desc = *new(mem) USB::DeviceDescriptor();
	*length = sizeof(USB::DeviceDescriptor);

	desc.bcdUSB = 0x0200;
	desc.classDescription = USB::UsbClassDescriptor(0,0,0);
	desc.bMaxPacketSize0 = 64;
	desc.idVendor = 0x0301;
	desc.idProduct = 0x0000;
	desc.bcdDevice = USB::UsbVersion(2);
	desc.iManufacturer = USBD_IDX_MFC_STR;
	desc.iProduct = USBD_IDX_PRODUCT_STR;
	desc.iSerialNumber = USBD_IDX_SERIAL_STR;
	desc.bNumConfigurations = 1;
	return reinterpret_cast<uint8_t*>(&desc);
}

struct USBDDummyClassData{
	uint32_t reserved;
};

static USBDDummyClassData _classData = {};

void UsbDevice::start(UsbDDeviceContext* context){
	_context = context;

	hUsbDevice.pClassData = &_classData; //Otherwise USBD_Reset handler would not disable interfaces ((
	//hUsbDevice.pClassData = nullptr; //Init?
	hUsbDevice.dev_speed = USBD_SPEED_FULL;

	USBD_Init(&hUsbDevice, const_cast<USBD_DescriptorsTypeDef*>(&_descriptorsTable), 0);

	USBD_RegisterClass(&hUsbDevice, const_cast<USBD_ClassTypeDef*>(&_usbClassBinding));
	USBD_Start(&hUsbDevice);

}
uint8_t* UsbDevice::coreGetDeviceQualifierDesc (uint16_t *length){
	*length = sizeof (USBD_NDC_DeviceQualifierDesc);
	return (uint8_t*)&USBD_NDC_DeviceQualifierDesc;
}
uint8_t* UsbDevice::coreGetUserStringDesc(USBD_HandleTypeDef* pdev, uint8_t index, uint16_t* length){
	*length = 0;
	if ( 0xEE == index ){
		*length = sizeof (NDC_StringDescriptor);
		return (uint8_t*)&NDC_StringDescriptor;
	}
	return NULL;
}

uint8_t* UsbDevice::getLangidStrDescriptor(USBD_SpeedTypeDef speed, uint16_t* length){
	auto mem = static_cast<void*>(&_descriptorsBuffer);
	auto& desc = *new(mem) USB::LanguageIDStringDescriptor<1>();
	*length = sizeof(USB::LanguageIDStringDescriptor<1>);
	desc.languages[0] = USB::LanguageID::EnglishUnitedStates;
	return reinterpret_cast<uint8_t*>(&desc);
}
uint8_t* UsbDevice::getManufacturerStringDescriptor(USBD_SpeedTypeDef speed, uint16_t* length){
	return USB::MakeStringDescriptor(_context->manufacturerStringDescriptor, &_descriptorsBuffer, sizeof(_descriptorsBuffer), length);
}
uint8_t* UsbDevice::getProductStringDescriptor(USBD_SpeedTypeDef speed, uint16_t* length){
	return USB::MakeStringDescriptor(_context->productStringDescriptor, &_descriptorsBuffer, sizeof(_descriptorsBuffer), length);
}
uint8_t* UsbDevice::getSerialStringDescriptor(USBD_SpeedTypeDef speed, uint16_t* length){
	return USB::MakeStringDescriptor(_context->serialStringDescriptor, &_descriptorsBuffer, sizeof(_descriptorsBuffer), length);
}
uint8_t* UsbDevice::getConfigStringDescriptor(USBD_SpeedTypeDef speed, uint16_t* length){
	return USB::MakeStringDescriptor(_context->configurationStringDescriptor, &_descriptorsBuffer, sizeof(_descriptorsBuffer), length);
}
uint8_t* UsbDevice::getInterfaceStringDescriptor(USBD_SpeedTypeDef speed, uint16_t* length){
	return USB::MakeStringDescriptor(_context->interfaceStringDescriptor, &_descriptorsBuffer, sizeof(_descriptorsBuffer), length);
}


USBD_StatusTypeDef UsbDevice::interfaceRequest(USBD_HandleTypeDef* pdev, USBD_SetupReqTypedef* req){
	uint8_t interface_id = (uint8_t)req->wValue; //TODO: bug! wIndex == interface id
	auto interface = _context->getInterface(interface_id);
	if(interface == nullptr){
		return USBD_FAIL;
	}
	return interface->interfaceRequest(pdev, req);
}




extern "C" USBD_StatusTypeDef USBD_hook_device_request(USBD_HandleTypeDef* pdev, USBD_SetupReqTypedef* req){
	if((req->bmRequest & USB_REQ_TYPE_MASK) == USB_REQ_TYPE_VENDOR){
		if(req->bRequest == (uint8_t)USB_VENDOR_CODE_WINUSB){
			if(req->wIndex == 0x04){
				USBD_CtlSendData (pdev, (uint8_t*)&_winusbFunctionDescriptor, req->wLength);
				return USBD_OK;
			}
		}
	}
	return USBD_FAIL;
}
extern "C" USBD_StatusTypeDef USBD_hook_interface_request(USBD_HandleTypeDef* pdev, USBD_SetupReqTypedef* req){
	return UsbDevice::interfaceRequest(pdev, req);
}


extern "C" void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd){
	USBD_HandleTypeDef* pdev = (USBD_HandleTypeDef*)hpcd->pData;

	//Hack to support WinUSB requests :(
	USBD_ParseSetupRequest(&pdev->request, (uint8_t *)hpcd->Setup);
	pdev->ep0_state = USBD_EP0_SETUP;
	pdev->ep0_data_len = pdev->request.wLength;

	switch (pdev->request.bmRequest & 0x1F) {
	case USB_REQ_RECIPIENT_DEVICE:
		if(USBD_hook_device_request(pdev, &pdev->request) == USBD_OK){
			return;
		}
		break;
	case USB_REQ_RECIPIENT_INTERFACE:
		if(USBD_hook_interface_request(pdev, &pdev->request) == USBD_OK){
			return;
		}
		break;
	}

	USBD_LL_SetupStage((USBD_HandleTypeDef*)hpcd->pData, (uint8_t *)hpcd->Setup);
}

extern "C" void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd){
	while(true){
		asm("nop");
	}
}

extern "C" void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd){
	USBD_SpeedTypeDef speed = USBD_SPEED_FULL;
	switch (hpcd->Init.speed){
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

extern "C" void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd){
	//Inform USB library that core enters in suspend Mode
	if(hpcd->pData != 0){
		USBD_LL_Suspend((USBD_HandleTypeDef*)hpcd->pData);
	}

	//Enter in STOP mode
	if (hpcd->Init.low_power_enable){
		//Set SLEEPDEEP bit and SleepOnExit of Cortex System Control Register
		SCB->SCR |= (uint32_t)((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
	}
}

extern "C" void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd){
	while(true){
		asm("nop");
	}
}

extern "C" void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum){
	while(true){
		asm("nop");
	}
}

extern "C" void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum){
	while(true){
		asm("nop");
	}
}

extern "C" void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd){
	while(true){
		asm("nop");
	}
}

extern "C" void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd){
	while(true){
		asm("nop");
	}
}
}

#endif
#endif
