cmake_minimum_required(VERSION 3.11)
project(instructions_check C)

set(CMAKE_C_STANDARD 11)
add_executable(instructions_check instructions_check.c unit_framework.h)
