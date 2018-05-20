cmake_minimum_required(VERSION 3.11)
project(virtual_machien C)

set(CMAKE_C_STANDARD 11)

add_executable(virtual_machien main.c instructions.h stack.h)
add_subdirectory(tests)