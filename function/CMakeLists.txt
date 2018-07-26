cmake_minimum_required(VERSION 3.9)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE ON)

project(function)
# source files
set(SOURCES "")
list(APPEND SOURCES "main.cpp")

# headers files
set(HEADERS "")
list(APPEND HEADERS "function.hpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

#enabled c++17 standard
set(CMAKE_CXX_FLAGS -std=c++17)
