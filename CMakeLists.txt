#add_library(LFramework STATIC)
#set_property(TARGET LFramework PROPERTY CXX_STANDARD 17)

target_include_directories(${PROJECT_NAME} PUBLIC "Src")
target_include_directories(${PROJECT_NAME} PUBLIC "Src/LFramework/USB/Device")
target_include_directories(${PROJECT_NAME} PUBLIC "Src/LFramework/USB")
add_subdirectory(Src/LFramework)

#target_compile_definitions(${PROJECT_NAME} PRIVATE ${STM32_DEVICE})
