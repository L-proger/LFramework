add_library(LFrameworkUsbHost INTERFACE )
add_library(LFramework::Usb::Host ALIAS LFrameworkUsbHost )


target_sources(LFrameworkUsbHost
INTERFACE
	IUsbDevice.h
	IUsbHostEndpoint.h
	IUsbInterface.h
	IUsbService.h
	IUsbTransfer.h
	UsbException.h
)


if(CMAKE_SYSTEM_NAME MATCHES "Linux")

target_sources(LFrameworkUsbHost
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


if(CMAKE_SYSTEM_NAME MATCHES "Windows")

target_link_libraries(LFrameworkUsbHost 
INTERFACE 
	Winusb 
	#User32 
	#Advapi32 
	#Ole32 
	SetupAPI
)


target_sources(LFrameworkUsbHost
INTERFACE
	Windows/USBHDevice.cpp
	Windows/UsbHDevice.h
	Windows/UsbHostEndpoint.h
	Windows/UsbHException.h
	Windows/UsbHInterface.h
	Windows/UsbHostManager.cpp
	Windows/UsbHostManager.h
	Windows/UsbNotifyWindow.h
	Windows/UsbService.cpp
	Windows/UsbService.h
)

endif()

