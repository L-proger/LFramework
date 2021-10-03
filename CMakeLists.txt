
if(DEFINED LFRAMEWORK_LIB)

set(PROJECT_NAME "LFramework")
add_library(LFramework STATIC)
set_property(TARGET LFramework PROPERTY CXX_STANDARD 17)

target_include_directories(${PROJECT_NAME} PRIVATE "${LFRAMEWORK_CONFIG_FILE_DIR}")

endif()



target_include_directories(${PROJECT_NAME} PUBLIC "Src")
target_include_directories(${PROJECT_NAME} PUBLIC "Src/LFramework/USB/Device")
target_include_directories(${PROJECT_NAME} PUBLIC "Src/LFramework/USB")
add_subdirectory(Src/LFramework)
