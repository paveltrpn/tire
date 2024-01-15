cmake_minimum_required(VERSION 3.28)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

project (tiny-render)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-g -Wall -Wextra -Wpedantic -stdlib=libc++ -Wno-unused-parameter)
add_link_options(-stdlib=libc++)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(EXTERN_LIBS "../extern_libs")

link_directories("${EXTERN_LIBS}/libjpeg/lib")
include_directories("${EXTERN_LIBS}/libjpeg/include")

link_directories("${EXTERN_LIBS}/libtga/lib")
include_directories("${EXTERN_LIBS}/libtga/include")

link_directories("${EXTERN_LIBS}/glfw/lib")
include_directories("${EXTERN_LIBS}/glfw/include")

include_directories("${EXTERN_LIBS}/glew/include")
link_directories("${EXTERN_LIBS}/glew/lib")

set(ALGEBRA_DIR "internal/algebra")
add_subdirectory(${ALGEBRA_DIR})
include_directories(${ALGEBRA_DIR})

add_subdirectory(src/001_fullhouse)
target_link_libraries(001_fullhouse algebra libjpeg tga glfw3 GL GLU GLEW)

add_subdirectory(src/004_fullhouse2)
target_link_libraries(004_fullhouse2 algebra libjpeg tga glfw3 GL GLU GLEW)

add_subdirectory(src/005_rocket)
target_link_libraries(005_rocket algebra libjpeg tga glfw3 GL GLU GLEW)

add_subdirectory(src/006_rocket_nix)
target_link_libraries(006_rocket_nix algebra libjpeg tga glfw3 GL GLU GLEW)

add_subdirectory(src/008_canvas)
target_link_libraries(008_canvas libjpeg tga)
