cmake_minimum_required(VERSION 3.0)

project (005_rocket)

set(CMAKE_CXX_COMPILER g++-13.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
-Wextra -Wpedantic -Wno-unused-parameter -std=c++20")

set(LIBS -lstdc++ -lGL -lGLU -lGLEW -lglfw3 -ltga -llibjpeg)

set(SOURCES main.cpp
            bitmap.cpp
            bitmap_text.cpp)

set(ALGEBRA_DIR ../../internal/algebra)
set(ALGEBRA_SRC ${ALGEBRA_DIR}/mtrx2.cpp
                ${ALGEBRA_DIR}/mtrx3.cpp
                ${ALGEBRA_DIR}/mtrx4.cpp
                ${ALGEBRA_DIR}/vec2.cpp
                ${ALGEBRA_DIR}/vec3.cpp
                ${ALGEBRA_DIR}/vec4.cpp
                ${ALGEBRA_DIR}/qtnn.cpp
                ${ALGEBRA_DIR}/plane.cpp)

include_directories(${ALGEBRA_DIR})
include_directories("../../extern/glfw/include")
include_directories("../../extern/glew-2.2.0/include")
include_directories("../../extern/libjpeg-cmake")
include_directories("../../extern/libtga/include")

link_directories("../../extern/libjpeg-cmake/build")
link_directories("../../extern/libtga/build/src")
link_directories("../../extern/glew-2.2.0/lib")
link_directories("../../extern/glfw/build/src")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ".")

add_executable(main ${SOURCES} ${ALGEBRA_SRC})

target_link_libraries(main ${LIBS})
