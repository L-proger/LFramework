cmake_minimum_required(VERSION 3.16)

add_library(LFramework INTERFACE)
target_include_directories(LFramework INTERFACE "Src")
add_subdirectory(Src/LFramework)
