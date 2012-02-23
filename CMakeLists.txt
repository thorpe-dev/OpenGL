cmake_minimum_required(VERSION 2.6)

PROJECT(cgRender)

set (CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -std=c++0x")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

include_directories($(cgRender_SOURCE_DIR)/include)
add_subdirectory(src)