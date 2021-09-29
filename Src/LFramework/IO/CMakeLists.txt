target_sources(${PROJECT_NAME} 
	PUBLIC
		Stream.h
		StreamWriter.h
	PRIVATE
		StreamWriter.cpp
)
add_subdirectory(Terminal)