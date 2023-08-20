add_library(LFrameworkIo INTERFACE )
add_library(LFramework::IO ALIAS LFrameworkIo )


target_sources(LFrameworkIo
INTERFACE
	Stream.h
	StreamWriter.h

	StreamWriter.cpp
)
add_subdirectory(Terminal)