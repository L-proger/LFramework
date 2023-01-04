target_sources(${PROJECT_NAME} 
INTERFACE
	IUsbDevice.h
	IUsbEndpoint.h
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
)

endif()

