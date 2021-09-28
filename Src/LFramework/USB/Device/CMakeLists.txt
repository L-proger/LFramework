target_sources(LFramework 
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


