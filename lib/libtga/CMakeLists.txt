cmake_minimum_required(VERSION 4.2)

project(libtga VERSION 1.0.1)

file(GLOB_RECURSE LIBTGA_SRC "src/*.c" "src/*.h" "include/*.h")
add_library(libtga)
target_sources(libtga PRIVATE ${LIBTGA_SRC})

# set_target_properties(libtga)

target_include_directories(libtga PUBLIC "include")
