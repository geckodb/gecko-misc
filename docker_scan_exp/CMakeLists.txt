cmake_minimum_required(VERSION 3.6)
project(docker_scan_exp)

set(CMAKE_C_STANDARD 11 -O3)

set(SOURCE_FILES main.c)
add_executable(docker_scan_exp ${SOURCE_FILES})