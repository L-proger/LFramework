#pragma once

#define HARD_FAULT_HANDLER \
__asm(  ".syntax unified\n" \
		"MOVS   R0, #4  \n" \
		"MOV    R1, LR  \n" \
		"TST    R0, R1  \n" \
		"BEQ    _MSP    \n" \
		"MRS    R0, PSP \n" \
		"B      HardFaultHandlerExtended      \n" \
		"_MSP:  \n" \
		"MRS    R0, MSP \n" \
		"B      HardFaultHandlerExtended      \n" \
		".syntax divided\n") ;
