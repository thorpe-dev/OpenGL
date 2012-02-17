cmake_minimum_required(VERSION 2.6)

PROJECT(cgRender)

set (CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

#find_package(BOOST REQUIRED)
#find_package(GLUT REQUIRED)
#find_package(OpenGL REQUIRED)
#find_package(VTK

#include_directories(${GLUT_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${BOOST_INCLUDE_DIRS})
include_directories($(cgRender_SOURCE_DIR)/include)
add_subdirectory(src)
 
