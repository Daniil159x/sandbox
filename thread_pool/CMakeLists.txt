cmake_minimum_required(VERSION 3.9)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE ON)

project(thread_pool)

#enabled c++11 standard
set(CMAKE_CXX_STANDARD 11)

#pthread
find_package(Threads REQUIRED)

# source files
set(SOURCES "")
list(APPEND SOURCES "main.cpp")
list(APPEND SOURCES "threadpool.cpp")

# headers files
set(HEADERS "")
list(APPEND HEADERS "threadpool.hpp")
list(APPEND HEADERS "utils.hpp")
list(APPEND HEADERS "task.hpp")
list(APPEND HEADERS "runnable.hpp")
list(APPEND HEADERS "outwrapper.hpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
