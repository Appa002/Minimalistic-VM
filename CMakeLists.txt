cmake_minimum_required(VERSION 3.11)
project(virtual_machien C)

set(CMAKE_C_STANDARD 11)

add_executable(virtual_machien main.c headers/instructions.h headers/stack.h headers/utils.h headers/object.h headers/object_maths.h)
add_subdirectory(tests)