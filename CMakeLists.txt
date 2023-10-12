cmake_minimum_required(VERSION 3.26)
project(bit_model C)

set(CMAKE_C_STANDARD 11)

add_executable(bit_model main.c
        decArr/decArr.h
        decArr/decArr.c)
