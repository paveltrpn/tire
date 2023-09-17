cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

project (tiny-render)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall \
                                        -Wextra \
                                        -Wpedantic \
                                        -Wno-unused-parameter \
                                        --std=c++2b \
                                        --stdlib=libc++")

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(LIBJPEG_DIR "./extern/libjpeg-cmake")
add_subdirectory(${LIBJPEG_DIR})
include_directories(${LIBJPEG_DIR})

set(LIBTGA_DIR "./extern/libtga")
add_subdirectory(${LIBTGA_DIR})
include_directories(${LIBTGA_DIR}/include)

set(GLFW_DIR "./extern/glfw")
add_subdirectory(${GLFW_DIR})
include_directories(${GLFW_DIR})

set(GLEW_DIR "./extern/glew-cmake")
add_subdirectory(${GLEW_DIR})
include_directories(${GLEW_DIR}/include)
link_directories(${GLEW_DIR}/build/lib)

set(ALGEBRA_DIR "./internal/algebra")
add_subdirectory(${ALGEBRA_DIR})
include_directories(${ALGEBRA_DIR})

set(LIBS -lGL -lGLU -lGLEW)

add_subdirectory(src/001_fullhouse)
target_link_libraries(001_fullhouse algebra libjpeg libtga glfw GL GLU GLEW)

add_subdirectory(src/004_fullhouse2)
target_link_libraries(004_fullhouse2 algebra libjpeg libtga glfw GL GLU GLEW)

add_subdirectory(src/005_rocket)
target_link_libraries(005_rocket algebra libjpeg libtga glfw GL GLU GLEW)

add_subdirectory(src/006_rocket_nix)
target_link_libraries(006_rocket_nix algebra libjpeg libtga glfw GL GLU GLEW)
