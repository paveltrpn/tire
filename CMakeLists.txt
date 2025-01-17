cmake_minimum_required(VERSION 3.28)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
add_compile_options(-stdlib=libc++)
add_link_options(-stdlib=libc++)
set(CMAKE_BUILD_TYPE Debug)

project (tiny_render)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON )

include_directories("modules")
#include_directories($ENV{NLOHMANN_JSON_DIR}/include)
#include_directories($ENV{LIBUV_DIR}/include)
#include_directories($ENV{VULKAN_SDK}/include)

if (DEFINED ENV{VULKAN_SDK} AND NOT "$ENV{VULKAN_SDK}" STREQUAL "")
    message( "VULKAN_SDK is set to $ENV{VULKAN_SDK}")
    include_directories($ENV{VULKAN_SDK}/include)
else()
    message(SEND_ERROR "VULKAN_SDK is not set")
endif()

if (DEFINED ENV{NLOHMANN_JSON_DIR} AND NOT "$ENV{NLOHMANN_JSON_DIR}" STREQUAL "")
    message( "NLOHMANN_JSON_DIR is set to $ENV{NLOHMANN_JSON_DIR}")
    include_directories($ENV{NLOHMANN_JSON_DIR}/include)
else()
    message(SEND_ERROR "NLOHMANN_JSON_DIR is not set")
endif()

if (DEFINED ENV{LIBUV_DIR} AND NOT "$ENV{LIBUV_DIR}" STREQUAL "")
    message("LIBUV_DIR is set to $ENV{LIBUV_DIR}")
    include_directories($ENV{LIBUV_DIR}/include)
    link_directories($ENV{LIBUV_DIR}/build)
else()
    message(SEND_ERROR "LIBUV_DIR is not set")
endif()

find_package(Vulkan REQUIRED)

add_library(log
    modules/log/log.h
    modules/log/log.cpp
)
#global user defined output messages macro definition
add_compile_definitions(ENABLE_DEBUG_OUTPUT=false)
add_compile_definitions(ENABLE_WARNING_OUTPUT=true)
add_compile_definitions(ENABLE_INFO_OUTPUT=true)
add_compile_definitions(ENABLE_ERROR_OUTPUT=true)
set_target_properties(log PROPERTIES LINKER_LANGUAGE CXX)

add_library(algebra
    modules/algebra/concepts.h
    modules/algebra/matrix.h
    modules/algebra/matrix2.h
    modules/algebra/matrix3.h
    modules/algebra/matrix4.h
    modules/algebra/vector.h
    modules/algebra/vector2.h
    modules/algebra/vector3.h
    modules/algebra/vector4.h
    modules/algebra/quaternion.h
)
set_target_properties(algebra PROPERTIES LINKER_LANGUAGE CXX)

add_library(config
    modules/config/config.h
    modules/config/config.cpp
)

add_library(render
    modules/render/render.cpp
    modules/render/render.h
    modules/render/gl/rendergl.cpp
    modules/render/gl/rendergl.h
    modules/render/gl/main_loop.cpp
    modules/render/gl/functions.cpp
    modules/render/gl/functions.h
    modules/render/gl/buffer.cpp
    modules/render/gl/buffer.h
    modules/render/gl/scene.h
    modules/render/gl/scene.cpp
    modules/render/gl/shader_storage.h
    modules/render/gl/shader_storage.cpp
    modules/render/gl/program.h
    modules/render/gl/program.cpp
    modules/render/gl/texture.h
    modules/render/gl/texture.cpp
    modules/render/vk/rendervk.cpp
    modules/render/vk/rendervk.h
    modules/render/vk/instance.cpp
    modules/render/vk/instance.h
    modules/render/vk/surface.cpp
    modules/render/vk/surface.h
    modules/render/vk/device.cpp
    modules/render/vk/device.h
    modules/render/vk/main_loop.cpp
    modules/render/vk/shader_storage.h
    modules/render/vk/shader_storage.cpp
    modules/render/vk/pipelines/pipeline.h
    modules/render/vk/pipelines/pipeline.cpp
    modules/render/vk/commands/commands.h
    modules/render/vk/commands/commands.cpp
    modules/render/vk/scene.h
    modules/render/vk/scene.cpp
    modules/render/vk/swapchain.h
    modules/render/vk/swapchain.cpp
    modules/render/vk/command_pool.h
    modules/render/vk/command_pool.cpp
)
target_link_libraries(render scene config geometry algebra)

add_library(image
    modules/image/bmp.cpp
    modules/image/bmp.h
    modules/image/tga.cpp
    modules/image/tga.h
    modules/image/canvas.cpp
    modules/image/canvas.h
    modules/image/color.cpp
    modules/image/color.h
)

add_library(geometry
    modules/geometry/transformations.cpp
    modules/geometry/transformations.h
    modules/geometry/point.cpp
    modules/geometry/point.h
    modules/geometry/normal.cpp
    modules/geometry/normal.h
    modules/geometry/wavefront.cpp
    modules/geometry/wavefront.h
    modules/geometry/polytope.cpp
    modules/geometry/polytope.h
)
target_link_libraries(geometry algebra)

add_library(scene
    modules/scene/scene.h
    modules/scene/scene.cpp
    modules/scene/node.cpp
    modules/scene/node.h
    modules/scene/camera.h
    modules/scene/camera.cpp
)
target_link_libraries(scene io image algebra geometry)

add_library(io
    modules/io/async.h
    modules/io/file.h
    modules/io/file.cpp
    modules/io/factory.h
    modules/io/factory.cpp
)
target_link_libraries(io)

add_subdirectory(test/window)
target_link_libraries(gl_test PUBLIC log io algebra scene config render X11 GL GLU Vulkan::Vulkan uv)
target_link_libraries(vk_test PUBLIC log io algebra scene config render X11 GL GLU Vulkan::Vulkan uv)

add_subdirectory(test/wavefront)
target_link_libraries(wavefront geometry)

add_subdirectory(test/color)
target_link_libraries(color image)
