cmake_minimum_required(VERSION 3.0)
project(docker_scan_exp)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -O3")

set(SOURCE_FILES main.c)
add_executable(docker_scan_exp ${SOURCE_FILES})
