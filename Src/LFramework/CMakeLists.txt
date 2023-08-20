target_sources(LFramework
INTERFACE
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

	TypeTraits/FunctionTraits.h
	TypeTraits/Integral.h

	Reflection/Reflection.h
)

add_subdirectory(USB)
add_subdirectory(UnitTest)
add_subdirectory(Threading)
add_subdirectory(IO)

#add_subdirectory(Time)

#add_subdirectory(MCU)
#add_subdirectory(Macro)
#add_subdirectory(IO)
#add_subdirectory(Input)
#add_subdirectory(Detect)
#add_subdirectory(Containers)
#add_subdirectory(COM)
