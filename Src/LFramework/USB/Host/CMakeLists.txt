target_sources(LFramework 
	PUBLIC
		UsbHDevice.h
		UsbHEndpoint.h
		UsbHException.h
		UsbHInterface.h
		UsbHostManager.h
		UsbNotifyWindow.h
		UsbService.h
	PRIVATE
		USBHDevice.cpp
		UsbHostManager.cpp
		UsbService.cpp
)

