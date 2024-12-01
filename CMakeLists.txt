cmake_minimum_required(VERSION 3.10)
project(Chess)

set(CMAKE_C_STANDARD 99)

# Add include directory for headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add source files
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)

# Create the executable
add_executable(main ${SOURCES})
