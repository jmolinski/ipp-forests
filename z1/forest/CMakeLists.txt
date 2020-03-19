cmake_minimum_required(VERSION 3.15)
project(forest C)

set(CMAKE_C_STANDARD 11)

add_executable(forest main.c input_interface.h input_interface.c commands.h commands.c bst.c bst.h output_interface.c output_interface.h safe_malloc.h)