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

add_library(toy_std)
target_sources(toy_std
  PUBLIC
    FILE_SET CXX_MODULES FILES
    toy_std/src/concepts.cpp
    toy_std/src/array.cpp
    toy_std/src/raw_list.cpp
    toy_std/src/stack.cpp
    toy_std/src/vector.cpp
    toy_std/src/raw_vector.cpp
    toy_std/src/algebra.cpp
    toy_std/src/algebra_matrix.cpp
    toy_std/src/algebra_quaternion.cpp
    toy_std/src/algebra_vector.cpp
    toy_std/src/iterator_traits.cpp
    toy_std/src/toy_std.cpp
    toy_std/src/algorithm.cpp
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

add_library(geometry)
target_sources(geometry
  PUBLIC
    FILE_SET CXX_MODULES FILES
    geometry/Polytope.cpp
)

target_link_libraries(screen render config)
target_link_libraries(geometry toy_std)

add_subdirectory(test/window)
target_link_libraries(window config screen render geometry glfw3 GL GLU GLEW vulkan)
