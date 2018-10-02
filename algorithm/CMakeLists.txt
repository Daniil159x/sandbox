cmake_minimum_required(VERSION 3.9)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE ON)

project(algorithm)

#enabled c++17 standard
set(CMAKE_CXX_STANDARD 17)

#pthread
find_package(Threads REQUIRED)

# source files
set(SOURCES "")
list(APPEND SOURCES "main.cpp")

# headers files
set(HEADERS "")
list(APPEND HEADERS "dfs.hpp")
list(APPEND HEADERS "bfs.hpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
