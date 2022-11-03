target_sources(${PROJECT_NAME} 
INTERFACE
	CriticalSection.h
	CriticalSectionFreeRTOS.h
	CriticalSectionStd.h
	Semaphore.h
	SemaphoreFreeRTOS.h
	SemaphoreStd.h
	Thread.h
	ThreadFreeRTOS.h
	ThreadPlatform.h
	ThreadStd.h

	CriticalSectionStd.cpp
	SemaphoreStdTest.cpp
	Thread.cpp
	ThreadTest.cpp
)


