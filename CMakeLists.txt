cmake_minimum_required(VERSION 3.20)
project(nbody)

# Commands 
# cmake -G "MinGW Makefiles" ..
# cmake --build .

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g")

# Output folders
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# ---------------------------------------------
# Main executable
# ---------------------------------------------
add_executable(nbody
    src/main.cpp
    src/bodies.cpp
    src/tree.cpp
)

