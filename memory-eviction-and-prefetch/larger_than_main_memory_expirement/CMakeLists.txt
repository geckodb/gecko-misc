cmake_minimum_required(VERSION 3.7)
project(tpch)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -O3)


include_directories(schema)
include_directories(utilities)
include_directories(tpch_queries)
include_directories(input_stream)

find_package(Boost 1.57.0)
include_directories(${Boost_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp)
add_executable(tpch ${SOURCE_FILES})
