cmake_minimum_required(VERSION 3.23)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

file(GLOB sources "src/*")
add_executable(untitled ${sources})

target_include_directories(untitled PUBLIC "include/")
