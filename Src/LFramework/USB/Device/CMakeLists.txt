add_library(LFrameworkUsbDevice INTERFACE )
add_library(LFramework::Usb::Device ALIAS LFrameworkUsbDevice )

target_sources(LFrameworkUsbDevice
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


target_compile_definitions(LFrameworkUsbDevice
INTERFACE
	USBD_SUPPORT_USER_STRING_DESC=1
)

target_include_directories(LFrameworkUsbDevice
INTERFACE
	.
)