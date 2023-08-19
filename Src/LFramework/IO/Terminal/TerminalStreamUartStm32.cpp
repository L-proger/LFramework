#include "TerminalStream.h"

#ifdef LF_TERMINAL_STREAM_USART_STM32

#include <usart.h>
#include <stm32f7xx_hal.h>

namespace LFramework::Terminal {
	int LFramework::Terminal::Stream::write(const char* data, int length) {
		if(HAL_OK == HAL_UART_Transmit(&huart1, reinterpret_cast<uint8_t*>(const_cast<char*>(data)), length, HAL_MAX_DELAY)){
			return length;
		}
		return 0;
	}
}

#endif
