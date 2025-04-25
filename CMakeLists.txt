cmake_minimum_required(VERSION 3.13)

project(taskium)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 23)

# Collect all source files
file(GLOB_RECURSE SRC_FILES
    core/*.cpp
    include/*.cpp
    src/*.cpp
    tests/*.cpp
)

# Add executable
add_executable(taskium tests/main.cpp ${SRC_FILES})

# Optional warnings
target_compile_options(taskium PRIVATE -Wall -Wextra -pedantic)
