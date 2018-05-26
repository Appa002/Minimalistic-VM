cmake_minimum_required(VERSION 3.11)
project(instructions_check C)

set(CMAKE_C_STANDARD 11)

file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/../source/*.c)
file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/../headers/*.h)

add_executable(instructions_check instructions_check.c  ${sources} ${headers} unit_framework.h)

