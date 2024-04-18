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

include_directories("modules")
include_directories("3rdparty")

add_library(config)
target_sources(config
  PUBLIC
    modules/config/Config.cpp
    modules/config/Config.h
)

# add_library(algebra)
# target_sources(algebra
  # PUBLIC
    # algebra/vectorBase.h
    # algebra/Vector.h
# )

add_library(render)
target_sources(render
  PUBLIC
    modules/render/Render.cpp
    modules/render/Render.h
    modules/render/RenderGL.cpp
    modules/render/RenderGL.h
    modules/render/RenderVK.cpp
    modules/render/RenderVK.h
)
target_link_libraries(render config)

add_library(geometry)
target_sources(geometry
  PUBLIC
    modules/geometry/WavefrontObj.cpp
    modules/geometry/WavefrontObj.h
    modules/geometry/WavefrontMtl.cpp
    modules/geometry/WavefrontMtl.h
    modules/geometry/Polytope.cpp
    modules/geometry/Polytope.h
)

target_link_libraries(render config)
target_link_libraries(geometry)

add_subdirectory(test/window)
target_link_libraries(window config render geometry X11 GL GLU vulkan)

add_subdirectory(test/algebra)
target_link_libraries(algebra_test boost_unit_test_framework)
