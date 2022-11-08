target_sources(${PROJECT_NAME} 
PUBLIC
	Assert.h
	BitField.h
	CMakeLists.txt
	Debug.h
	Guid.h
	GuidUtils.h
	GuidUtils.cpp
	System.h
	SystemArmCortexM.h
	SystemWindows.h
	TupleUtils.h

	BitFieldTest.cpp
	Syscalls.cpp

	Text/Encoding.h
)

add_subdirectory(USB)
add_subdirectory(UnitTest)
add_subdirectory(TypeTraits)
add_subdirectory(Time)
add_subdirectory(Threading)
add_subdirectory(Reflection)
add_subdirectory(MCU)
add_subdirectory(Macro)
add_subdirectory(IO)
add_subdirectory(Input)
add_subdirectory(Detect)
add_subdirectory(Containers)
add_subdirectory(COM)
