cmake_minimum_required(VERSION 3.26)
project(bug C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(bug
        main.c)
