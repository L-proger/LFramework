add_library(LFrameworkUnitTest INTERFACE )
add_library(LFramework::UnitTest ALIAS LFrameworkUnitTest )

target_sources(LFrameworkUnitTest
INTERFACE
	UnitTest.h
	UnitTest.cpp
)
