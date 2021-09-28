target_sources(LFramework 
	PUBLIC
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
	PRIVATE
		CriticalSectionStd.cpp
		SemaphoreStdTest.cpp
		Thread.cpp
		ThreadTest.cpp
)


