add_library(LFrameworkIoTerminal INTERFACE )
add_library(LFramework::IO::Terminal ALIAS LFrameworkIoTerminal )

target_sources(LFrameworkIoTerminal
INTERFACE
	Terminal.h
	TerminalAnsi.h
	TerminalStream.h

	Terminal.cpp
	TerminalStreamCout.cpp
	TerminalStreamUartStm32.cpp
)