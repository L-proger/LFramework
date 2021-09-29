target_sources(${PROJECT_NAME} 
	PUBLIC
		UsbDBulkInterface.h
		UsbDDeviceContext.h
		UsbDEndpoint.h
		USBDevice.h
		UsbDInterface.h
	PRIVATE
		UsbDEndpoint.cpp
		USBDevice.cpp
		UsbDLowLevel.cpp
)


