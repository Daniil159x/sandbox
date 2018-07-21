cmake_minimum_required(VERSION 3.9)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE ON)

project(function)
add_executable(${PROJECT_NAME} "main.cpp")

#enabled c++17 standard
set(CMAKE_CXX_FLAGS -std=c++17)
