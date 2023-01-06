target_sources(${PROJECT_NAME} 
INTERFACE
	IUsbDevice.h
	IUsbHostEndpoint.h
	IUsbInterface.h
	IUsbService.h
	IUsbTransfer.h
	UsbException.h
)


if(CMAKE_SYSTEM_NAME MATCHES "Linux")

target_sources(${PROJECT_NAME} 
INTERFACE
	Linux/NetlinkClient.h
	Linux/NetlinkClient.cpp
	Linux/NetlinkReader.h
	Linux/UsbService.h
	Linux/UsbService.cpp
	Linux/UsbDevice.h
	Linux/UsbDevice.cpp
	Linux/UsbInterface.h
	Linux/UsbHostEndpoint.h
	Linux/UsbHostEndpoint.cpp
	Linux/UsbIoctl.h
	Linux/UsbBulkTransfer.h
)


endif()

