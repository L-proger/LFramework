#include "TerminalStream.h"

#ifdef LF_TERMINAL_STREAM_USART_STM32


#ifdef STM32F7
#include <stm32f7xx_hal.h>
#include <stm32f7xx_ll_usart.h>
#elif STM32H7
#include <stm32h7xx_hal.h>
#include <stm32h7xx_ll_usart.h>
#endif

namespace LFramework::Terminal {


	int LFramework::Terminal::Stream::write(const char* data, int length) {
		for(int i = 0; i < length; ++i){
			while (!LL_USART_IsActiveFlag_TXE(LF_TERMINAL_STREAM_USART)) {}
			LL_USART_TransmitData9(LF_TERMINAL_STREAM_USART, *data);
			++data; 
		}
		
		//if(HAL_OK == HAL_UART_Transmit(&huart1, reinterpret_cast<uint8_t*>(const_cast<char*>(data)), length, HAL_MAX_DELAY)){
		//	return length;
		//}
		return length;
	}
}

#endif
