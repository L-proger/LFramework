target_sources(${PROJECT_NAME} 
INTERFACE
	Stream.h
	StreamWriter.h

	StreamWriter.cpp
)
add_subdirectory(Terminal)