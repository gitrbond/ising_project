cmake_minimum_required(VERSION 3.12)  # CMake version check
#set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project(ising)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

# Add main.cpp file of project root directory as source file
set(SOURCE_FILES main.cpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(ising ${SOURCE_FILES})