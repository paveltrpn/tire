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

include_directories("${EXTERN_LIBS}/json/include")

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

add_library(config)
target_sources(config
  PUBLIC
    FILE_SET CXX_MODULES FILES
    config/Config.cpp
)

add_library(render)
target_sources(render
  PUBLIC
    FILE_SET CXX_MODULES FILES
    render/IRender.cpp
    render/Render.cpp
    render/RenderGL.cpp
    render/RenderVK.cpp
    render/RenderVKDisplay.cpp
)
target_link_libraries(render config)

add_library(screen)
target_sources(screen
  PUBLIC
    FILE_SET CXX_MODULES FILES
    screen/IScreen.cpp
    screen/Screen.cpp
    screen/X11Screen.cpp
    screen/GLFWScreen.cpp
)
target_link_libraries(screen render config)

add_subdirectory(test/window)
target_link_libraries(window config screen render glfw3 GL GLU GLEW vulkan)
