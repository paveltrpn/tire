cmake_minimum_required(VERSION 3.28)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_SCAN_FOR_MODULES)

project (tiny-render)

set(CMAKE_CXX_STANDARD 23)
add_compile_options(-g -O0 -Wall -Wextra -Wpedantic -stdlib=libc++ -Wno-unused-parameter)
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

set(TOY_STL "toy-stl")
add_library(toy_stl)
target_sources(toy_stl
  PUBLIC
    FILE_SET CXX_MODULES FILES
    ${TOY_STL}/src/concepts.cpp
    ${TOY_STL}/src/array.cpp
    ${TOY_STL}/src/raw_list.cpp
    ${TOY_STL}/src/stack.cpp
    ${TOY_STL}/src/vector.cpp
    ${TOY_STL}/src/raw_vector.cpp
    ${TOY_STL}/src/algebra.cpp
    ${TOY_STL}/src/algebra_matrix.cpp
    ${TOY_STL}/src/algebra_quaternion.cpp
    ${TOY_STL}/src/algebra_vector.cpp
    ${TOY_STL}/src/iterator_traits.cpp
    ${TOY_STL}/src/toy_std.cpp
    ${TOY_STL}/src/algorithm.cpp
)

add_library(screen)
target_sources(screen
  PUBLIC
    FILE_SET CXX_MODULES FILES
    screen/IScreen.cpp
    screen/Screen.cpp
    screen/X11Screen.cpp
    screen/GLFWScreen.cpp
    screen/Render.cpp
    screen/RenderGL.cpp
    screen/RenderVK.cpp
    screen/RenderSF.cpp
)

add_subdirectory(test/glfw_window)
target_link_libraries(glfw_window screen glfw3 GL GLU GLEW vulkan)

#add_subdirectory(src/004_fullhouse2)
#target_link_libraries(004_fullhouse2 algebra libjpeg tga glfw3 GL GLU GLEW)

#add_subdirectory(src/005_rocket)
#target_link_libraries(005_rocket algebra libjpeg tga glfw3 GL GLU GLEW)

#add_subdirectory(src/006_rocket_nix)
#target_link_libraries(006_rocket_nix algebra libjpeg tga glfw3 GL GLU GLEW)

#add_subdirectory(src/008_canvas)
#target_link_libraries(008_canvas libjpeg tga)
