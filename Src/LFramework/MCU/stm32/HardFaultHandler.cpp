#ifndef _WIN32


#include <LFramework/Debug.h>
#include <LFramework/IO/StreamWriter.h>

extern "C" {
	void HardFaultHandlerExtended(unsigned int * hardfault_args){
		unsigned int stacked_r0;
		unsigned int stacked_r1;
		unsigned int stacked_r2;
		unsigned int stacked_r3;
		unsigned int stacked_r12;
		unsigned int stacked_lr;
		unsigned int stacked_pc;
		unsigned int stacked_psr;

		stacked_r0 = ((unsigned int)hardfault_args[0]);
		stacked_r1 = ((unsigned int)hardfault_args[1]);
		stacked_r2 = ((unsigned int)hardfault_args[2]);
		stacked_r3 = ((unsigned int)hardfault_args[3]);

		stacked_r12 = ((unsigned int)hardfault_args[4]);
		stacked_lr = ((unsigned int)hardfault_args[5]);
		stacked_pc = ((unsigned int)hardfault_args[6]);
		stacked_psr = ((unsigned int)hardfault_args[7]);

		lfFatal() << "----------";
		lfFatal() << "Hard fault";
		lfFatal() << "----------";

		lfFatal() << "stacked_lr: 0x" << LFramework::HexFormat(stacked_lr) ;
		lfFatal() << "stacked_pc: 0x" << LFramework::HexFormat(stacked_pc) ;

		while(true){
			asm("nop");
		}

	}
}
#endif
