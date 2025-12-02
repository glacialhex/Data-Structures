cmake_minimum_required(VERSION 3.10)

# Project name
project(Data_Structures)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable with all source files
add_executable(Data_Structures
    Main.cpp
    Node.cpp
    Node.h
    RadixTree.cpp
    RadixTree.h
)

# Optional: Add compiler warnings for better code quality
if(MSVC)
    target_compile_options(Data_Structures PRIVATE /W4)
else()
    target_compile_options(Data_Structures PRIVATE -Wall -Wextra -pedantic)
endif()

