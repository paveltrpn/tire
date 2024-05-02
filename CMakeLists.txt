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

add_library(render)
target_sources(render
  PUBLIC
    modules/render/Render.cpp
    modules/render/Render.h
    modules/render/RenderLegacyGL.cpp
    modules/render/RenderLegacyGL.h
    modules/render/RenderGL.cpp
    modules/render/RenderGL.h
    modules/render/RenderVK.cpp
    modules/render/RenderVK.h
    modules/render/GLFunctions.cpp
    modules/render/GLFunctions.h
)
target_link_libraries(render config)

add_library(assets)
target_sources(assets
  PUBLIC
    modules/assets/Wavefront.cpp
    modules/assets/Wavefront.h
    modules/assets/Polytope.cpp
    modules/assets/Polytope.h
    modules/assets/Png.cpp
    modules/assets/Png.h
)

add_library(camera)
target_sources(camera
  PUBLIC
    modules/camera/Camera.cpp
    modules/camera/Camera.h
)

add_library(shader)
target_sources(shader
  PUBLIC
    modules/shader/Shader.cpp
    modules/shader/Shader.h
    modules/shader/ShaderSources.cpp
    modules/shader/ShaderSources.h
)

add_library(geometry)
target_sources(geometry
  PUBLIC
    modules/geometry/Node.cpp
    modules/geometry/Node.h
    modules/geometry/Transformations.cpp
    modules/geometry/Transformations.h
    modules/geometry/Point.cpp
    modules/geometry/Point.h
    modules/geometry/Normal.cpp
    modules/geometry/Normal.h
)

target_link_libraries(render config shader)
target_link_libraries(assets)

add_subdirectory(test/window)
target_link_libraries(gl config render shader assets X11 GL GLU vulkan)
target_link_libraries(vk config render shader assets X11 GL GLU vulkan)
target_link_libraries(lg config render shader assets X11 GL GLU vulkan)
