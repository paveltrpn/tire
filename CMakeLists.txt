cmake_minimum_required(VERSION 3.28)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
add_compile_options(-stdlib=libc++ -std=c++23)
add_link_options(-stdlib=libc++)
set(CMAKE_BUILD_TYPE Debug)

project (tiny_render)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON )

include_directories("modules")

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

# Today is no physics
# if (DEFINED ENV{JOLT_PHYSICS_DIR} AND NOT "$ENV{JOLT_PHYSICS_DIR}" STREQUAL "")
#     message("JOLT_PHYSICS_DIR is set to $ENV{JOLT_PHYSICS_DIR}")
#     include_directories($ENV{JOLT_PHYSICS_DIR})
#     link_directories($ENV{JOLT_PHYSICS_DIR}/Build/Linux_Release)
# else()
#     message(SEND_ERROR "JOLT_PHYSICS_DIR is not set")
# endif()

find_package(Vulkan REQUIRED)

# Module log
# ================================
add_library(log
    modules/log/log.h
    modules/log/log.cpp
)
#global user defined output messages macro definition
add_compile_definitions(ENABLE_DEBUG_OUTPUT=false)
add_compile_definitions(ENABLE_WARNING_OUTPUT=true)
add_compile_definitions(ENABLE_INFO_OUTPUT=true)
add_compile_definitions(ENABLE_NOTICE_OUTPUT=true)
add_compile_definitions(ENABLE_ERROR_OUTPUT=true)
set_target_properties(log PROPERTIES LINKER_LANGUAGE CXX)

# Module algebra
# ================================
file(GLOB MOD_ALGEBRA_SRC "modules/algebra/*.cpp" "modules/algebra/*.h")
add_library(algebra)
target_sources(algebra PRIVATE ${MOD_ALGEBRA_SRC})
set_target_properties(algebra PROPERTIES LINKER_LANGUAGE CXX)

# Module ui
# ================================
file(GLOB MOD_UI_SRC "modules/ui/*.cpp" "modules/ui/*.h")
add_library(ui)
target_sources(ui PRIVATE ${MOD_UI_SRC})
set_target_properties(ui PROPERTIES LINKER_LANGUAGE CXX)

# Module config
# ================================
add_library(config)
target_sources(config
  PUBLIC
    FILE_SET CXX_MODULES FILES
    modules/config/interface.cpp
    modules/config/config.cpp
)

# Module render
# ================================
file(GLOB_RECURSE MOD_RENDER_SRC "modules/render/*.cpp" "modules/render/*.h")
add_library(render)
target_sources(render PRIVATE ${MOD_RENDER_SRC})
target_link_libraries(render ui scene config geometry algebra event)

# Module image
# ================================
file(GLOB MOD_IMAGE_SRC "modules/image/*.cpp" "modules/image/*.h")
add_library(image)
target_sources(image PRIVATE ${MOD_IMAGE_SRC})

# Module geometry
# ================================
file(GLOB MOD_GEOMETRY_SRC "modules/geometry/*.cpp" "modules/geometry/*.h")
add_library(geometry)
target_sources(geometry PRIVATE ${MOD_GEOMETRY_SRC})
target_link_libraries(geometry algebra)

# Module scene
# ================================
file(GLOB MOD_SCENE_SRC "modules/scene/*.cpp" "modules/scene/*.h")
add_library(scene)
target_sources(scene PRIVATE ${MOD_SCENE_SRC})
target_link_libraries(scene config io image algebra geometry)

# Module event
# ================================
add_library(event)
target_sources(event
  PUBLIC
    FILE_SET CXX_MODULES FILES
    modules/event/interface.cpp
    modules/event/async.cpp
    modules/event/context.cpp
    modules/event/timer.cpp
)
target_link_libraries(event config)

# Module io
# ================================
add_library(io)
target_sources(io
  PUBLIC
    FILE_SET CXX_MODULES FILES
    modules/io/interface.cpp
    modules/io/file.cpp
)
target_link_libraries(io event)

# Add subdirectories
# ================================
add_subdirectory(test/window)
target_link_libraries(gl_test PUBLIC 
    log 
    event 
    io 
    algebra 
    scene 
    config 
    render 
    X11
    Xfixes
    GL 
    GLU 
    Vulkan::Vulkan 
    uv
)

target_link_libraries(vk_test PUBLIC 
    log 
    event 
    io 
    algebra 
    scene 
    config 
    render 
    X11
    Xfixes
    GL 
    GLU 
    Vulkan::Vulkan 
    uv
)

set_target_properties( vk_test gl_test
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "../../../bin"
)

add_subdirectory(test/wavefront)
target_link_libraries(wavefront geometry)

add_subdirectory(test/color)
target_link_libraries(color image)
