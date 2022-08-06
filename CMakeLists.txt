cmake_minimum_required(VERSION 3.23)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("deps/untitled-argparser")
add_subdirectory("deps/untitled-logger")

file(GLOB sources "src/*")
add_executable(untitled ${sources})

target_include_directories(untitled PUBLIC ${ARGPARSER_INCLUDES} ${LOGGER_INCLUDES} "include/")
target_link_libraries(untitled PUBLIC ${ARGPARSER_LIBS} ${LOGGER_LIBS})
