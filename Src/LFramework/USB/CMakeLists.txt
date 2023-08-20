

add_library(LFrameworkUsb INTERFACE )
add_library( LFramework::Usb ALIAS LFrameworkUsb )

target_sources(LFrameworkUsb
INTERFACE
	USBDevice_Deprecated.h
	usbd_conf.h
	UsbEndpoint.h
	UsbException.h
	UsbInterface.h
	UsbMicrosoftTypes.h
	UsbPollingTransfer.h
	UsbTransfer.h
	USBTypes.h
	USBTypesTest.cpp
)

add_subdirectory(Device)
add_subdirectory(Host)


target_include_directories(LFrameworkUsb
INTERFACE
	.
)
