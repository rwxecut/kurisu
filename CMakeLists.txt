cmake_minimum_required(VERSION 3.10)
project(kurisu-example)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

set(INCLUDE_DIR include)
set(SDL2_PATH SDL2)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

set(SOURCE_FILES example.cpp Kurisu.cpp)
set(HEADER_FILES Kurisu.h)

include_directories(
        ${INCLUDE_DIR}
        ${SDL2_INCLUDE_DIR}
        ${OPENGL_INCLUDE_DIR})

add_executable(kurisu-example ${SOURCE_FILES})
target_link_libraries(kurisu-example ${SDL2_LIBRARY} ${OPENGL_LIBRARIES})
