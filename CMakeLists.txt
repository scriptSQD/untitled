cmake_minimum_required(VERSION 3.12)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("deps/untitled-argparser")
add_subdirectory("deps/untitled-logger")
add_subdirectory("deps/wxWidgets")
add_subdirectory("deps/libpqxx")

file(GLOB_RECURSE untitled_sources CONFIGURE_DEPENDS "src/**")
message("Sources are: ${untitled_sources}")

add_executable(untitled ${untitled_sources})

if (WIN32)
set_target_properties(untitled PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)
endif()

target_include_directories(untitled PUBLIC
        ${ARGPARSER_INCLUDES}
        ${LOGGER_INCLUDES}
        "include/"
)

target_link_libraries(untitled PUBLIC
        ${ARGPARSER_LIBS}
        ${LOGGER_LIBS}
        wx::core
        wx::base
        wx::net
        pqxx
)

target_precompile_headers(untitled PUBLIC "include/UntitledPrecomp.hpp")
