target_sources(${PROJECT_NAME} 
INTERFACE
	Mcu.h
)



add_subdirectory(DMA)
add_subdirectory(stm32)
add_subdirectory(USBDevice)