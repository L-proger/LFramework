#pragma once

#include <stm32f7xx_hal.h>
#include <usbd_def.h>
#include "UsbDDeviceContext.h"

namespace LFramework::USB {

class UsbDevice {
public:
	friend class UsbDEndpoint;

	static void start(UsbDDeviceContext* context);

	static UsbDDeviceContext* getContext();

	static USBD_StatusTypeDef interfaceRequest(USBD_HandleTypeDef* pdev, USBD_SetupReqTypedef* req);

	//Descriptors
	static uint8_t* getDeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);
	static uint8_t* getLangidStrDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);
	static uint8_t* getManufacturerStringDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);
	static uint8_t* getProductStringDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);
	static uint8_t* getSerialStringDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);
	static uint8_t* getConfigStringDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);
	static uint8_t* getInterfaceStringDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);

	//USB device core interface
	static uint8_t* coreGetCfgDesc(uint16_t* length);
	static uint8_t  coreSof(USBD_HandleTypeDef* pdev);
	static uint8_t  coreIsoInIncomplete(USBD_HandleTypeDef* pdev, uint8_t epnum);
	static uint8_t  coreIsoOutIncomplete(USBD_HandleTypeDef* pdev, uint8_t epnum);
	static uint8_t  coreSetup(USBD_HandleTypeDef* pdev, USBD_SetupReqTypedef *req);
	static uint8_t  coreEp0RxReady(USBD_HandleTypeDef* pdev);
	static uint8_t  coreDataIn(USBD_HandleTypeDef* pdev, uint8_t epnum);
	static uint8_t  coreDataOut(USBD_HandleTypeDef* pdev, uint8_t epnum);
	static uint8_t* coreGetDeviceQualifierDesc (uint16_t *length);
	static uint8_t  coreInit(USBD_HandleTypeDef* pdev, uint8_t cfgidx);
	static uint8_t  coreDeinit(USBD_HandleTypeDef* pdev, uint8_t cfgidx);
	static uint8_t* coreGetUserStringDesc(USBD_HandleTypeDef* pdev, uint8_t index, uint16_t* length);
	static uint8_t coreImplSetup(USBD_SetupReqTypedef request, void* data);
private:
	static UsbDInterface* findInterfaceByEndpointAddress(uint8_t address);
	static const USBD_DescriptorsTypeDef _descriptorsTable;
	static const USBD_ClassTypeDef _usbClassBinding;
	static USBD_HandleTypeDef hUsbDevice;
};

}
