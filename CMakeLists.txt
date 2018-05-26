cmake_minimum_required(VERSION 3.11)
project(virtual_machien C)

set(CMAKE_C_STANDARD 11)


file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/source/*.c)
file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/headers/*.h)

add_executable(virtual_machien main.c ${sources} ${headers})
add_subdirectory(tests)