add_library(LFramework STATIC)
set_property(TARGET LFramework PROPERTY CXX_STANDARD 17)
target_include_directories(LFramework PUBLIC Src PRIVATE ${LFRAMEWORK_CONFIG_FILE_DIR})
add_subdirectory(Src/LFramework)