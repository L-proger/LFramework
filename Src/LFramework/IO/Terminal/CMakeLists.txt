target_sources(${PROJECT_NAME} 
INTERFACE
	Terminal.h
	TerminalAnsi.h
	TerminalStream.h

	Terminal.cpp
	TerminalStreamCout.cpp
	TerminalStreamUartStm32.cpp
)