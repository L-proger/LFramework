target_sources(LFramework 
	PUBLIC
		Stream.h
		StreamWriter.h
	PRIVATE
		StreamWriter.cpp
)
add_subdirectory(Terminal)