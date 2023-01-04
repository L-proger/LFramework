target_sources(${PROJECT_NAME} 
INTERFACE
	UsbDBulkInterface.h
	UsbDDeviceContext.h
	UsbDEndpoint.h
	UsbDDevice.h
	UsbDInterface.h

	UsbDEndpoint.cpp
	USBDDevice.cpp
	UsbDLowLevel.cpp
	UsbDDeviceCallbacks.cpp
)


