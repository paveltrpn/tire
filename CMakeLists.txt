cmake_minimum_required(VERSION 3.28)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
add_compile_options(-g 
                    -Wall 
                    -Wextra 
                    -Wpedantic 
                    -stdlib=libc++
                    -Wno-unused-parameter)

add_link_options(-stdlib=libc++)

project (tiny_render)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

include_directories("modules")
include_directories("3rdparty")

add_subdirectory(modules/toy_std)

add_library(config)
target_sources(config
  PUBLIC
    FILE_SET CXX_MODULES FILES
        modules/config/Config.cpp
)

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
target_link_libraries(render camera opengl_types config assets toy_std)

add_library(assets)
target_sources(assets
  PUBLIC
    modules/assets/Wavefront.cpp
    modules/assets/Wavefront.h
    modules/assets/Polytope.cpp
    modules/assets/Polytope.h
    modules/assets/ShaderSources.cpp
    modules/assets/ShaderSources.h
)
target_link_libraries(assets toy_std)

add_library(image)
target_sources(image
  PUBLIC
    FILE_SET CXX_MODULES FILES
    modules/image/image.cpp
    modules/image/bmp.cpp
    modules/image/tga.cpp
    modules/image/canvas.cpp
)

add_library(camera)
target_sources(camera
    PUBLIC
      FILE_SET CXX_MODULES FILES
        modules/camera/Camera.cpp
)
target_link_libraries(camera toy_std)

add_library(opengl_types)
target_sources(opengl_types
  PUBLIC
      FILE_SET CXX_MODULES FILES
        modules/opengl_types/opengl_types.cpp
        modules/opengl_types/GLFunctions.cpp
        modules/opengl_types/Shader.cpp
)
target_link_libraries(opengl_types toy_std)

add_library(vulkan_types)
target_sources(vulkan_types
  PUBLIC
      FILE_SET CXX_MODULES FILES
        modules/vulkan_types/VulkanContext.cpp
)
target_link_libraries(vulkan_types)

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
target_link_libraries(geometry toy_std)

add_subdirectory(test/window)
target_link_libraries(gl camera opengl_types config render assets X11 GL GLU vulkan)
target_link_libraries(vk camera vulkan_types config render assets X11 GL GLU vulkan)

add_subdirectory(test/wavefront)
target_link_libraries(wavefront assets)

