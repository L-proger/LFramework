add_library(LFramework STATIC)
set_property(TARGET LFramework PROPERTY CXX_STANDARD 17)

file(GLOB_RECURSE SRC_FILES    
    "*.cpp"
)

file(GLOB_RECURSE HEADER_FILES    
    "*.h"
)

target_sources(LFramework PRIVATE ${SRC_FILES} PUBLIC ${HEADER_FILES})

target_include_directories(LFramework PUBLIC Src PRIVATE ${LFRAMEWORK_CONFIG_FILE_DIR})