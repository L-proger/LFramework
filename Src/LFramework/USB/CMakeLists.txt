target_sources(${PROJECT_NAME} 
	PUBLIC
		USBDevice_Deprecated.h
		usbd_conf.h
		UsbEndpoint.h
		UsbException.h
		UsbInterface.h
		UsbMicrosoftTypes.h
		UsbPollingTransfer.h
		UsbTransfer.h
		USBTypes.h
	PRIVATE
		USBTypesTest.cpp
)

add_subdirectory(Device)
add_subdirectory(Host)
