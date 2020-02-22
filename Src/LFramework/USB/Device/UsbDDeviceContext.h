#pragma once

#include "UsbInterface.h"

#include <cstdint>
#include <cstddef>
#include "UsbDInterface.h"

namespace LFramework::USB {

class UsbDDeviceContext {
public:

	UsbDDeviceContext(UsbDInterface* usbInterface) : _interface(usbInterface){}

	size_t getInterfaceCount() {
		return 1;
	}
	UsbDInterface* getInterface(size_t id) {
		return _interface;
	}

	const char* manufacturerStringDescriptor = nullptr;
	const char* productStringDescriptor = nullptr;
	const char* serialStringDescriptor = nullptr;
	const char* configurationStringDescriptor = nullptr;
	const char* interfaceStringDescriptor = nullptr;

private:
	UsbDInterface* _interface = nullptr;

	/*static uint8_t* getDeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);
	static uint8_t* getLangidStrDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);
	static uint8_t* getManufacturerStringDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);
	static uint8_t* getProductStringDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);
	static uint8_t* getSerialStringDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);
	static uint8_t* getConfigStringDescriptor(USBD_SpeedTypeDef speed, uint16_t* length);
	static uint8_t* getInterfaceStringDescriptor(USBD_SpeedTypeDef speed, uint16_t* lengt*/
};
}
