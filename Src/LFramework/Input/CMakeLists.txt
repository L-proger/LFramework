add_library(LFrameworkInput INTERFACE )
add_library(LFramework::Input ALIAS LFrameworkInput )

target_sources(LFrameworkInput
INTERFACE
	Input.h
	Keyboard.h
	KeysStateManager.h
	Mouse.h

	Input.cpp
	Keyboard.cpp
	Mouse.cpp
)