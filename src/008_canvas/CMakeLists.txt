cmake_minimum_required(VERSION 3.0)

project (008_canvas)

set(CMAKE_CXX_COMPILER g++-13.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
-Wextra -Wpedantic -Wno-unused-parameter -std=c++20")

set(LIBS -lstdc++ -ltga -llibjpeg)

set(SOURCES test.cpp
            canvas.cpp)

include_directories("../../extern/libjpeg-cmake")
include_directories("../../extern/libtga/include")

link_directories("../../extern/libjpeg-cmake/build")
link_directories("../../extern/libtga/build/src")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ".")

add_executable(main ${SOURCES})

target_link_libraries(main ${LIBS})
