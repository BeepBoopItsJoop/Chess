cmake_minimum_required(VERSION 3.10)
project(Chess)

set(CMAKE_C_STANDARD 99)

# SDL and SDL Libraries
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Add include directory for headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Assets
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Add source files
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)

# Create the executable
add_executable(chess ${SOURCES})
target_link_libraries(chess ${SDL2_LIBRARIES} SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)
