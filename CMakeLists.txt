cmake_minimum_required(VERSION 3.10)

# Project Name
project(RadixTreeStructure)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source files
set(SOURCES
    Main.cpp
    RadixTree.cpp
    Node.cpp
)

# Header files (optional, but good for IDEs)
set(HEADERS
    RadixTree.h
    Node.h
)

# Create executable
add_executable(RadixTreeApp ${SOURCES} ${HEADERS})
