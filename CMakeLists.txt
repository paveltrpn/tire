cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_COMPILER $ENV{GCC_LATEST}/bin/gcc-15.1)
set(CMAKE_CXX_COMPILER $ENV{GCC_LATEST}/bin/g++-15.1)
# set(CMAKE_C_COMPILER clang)
# set(CMAKE_CXX_COMPILER clang++)
# add_link_options(-stdlib=libc++)
add_compile_options(-std=c++26)
set(CMAKE_BUILD_TYPE Debug)

project (tiny_render)

set(CMAKE_CXX_STANDARD 26)
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

if (DEFINED ENV{LOCAL_GLFW} AND NOT "$ENV{LOCAL_GLFW}" STREQUAL "")
    message( "LOCAL_GLFW is set to $ENV{LOCAL_GLFW}")
    include_directories($ENV{LOCAL_GLFW}/include)
    link_directories($ENV{LOCAL_GLFW}/lib)
else()
    message(SEND_ERROR "LOCAL_GLFW is not set")
endif()

find_package(Vulkan REQUIRED)

set(QT_VERSION 6.10)
list(APPEND CMAKE_PREFIX_PATH $ENV{LOCAL_LATEST_QT})
find_package(Qt6 ${QT_VERSION} COMPONENTS
    Core
    Gui
    GuiPrivate
    Quick
    QuickWidgets
    QuickControls2
    Qml
)

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
file(GLOB MOD_UI_SRC "modules/ui/*.cpp")
add_library(ui)
target_sources(ui 
  PUBLIC
    FILE_SET CXX_MODULES FILES
    ${MOD_UI_SRC}
)

# Module config
# ================================
file(GLOB MOD_CONFIG_SRC "modules/config/*.cpp" "modules/config/*.h")
add_library(config)
target_sources(config PRIVATE ${MOD_CONFIG_SRC})

# Module render
# ================================
file(GLOB_RECURSE MOD_RENDER_SRC "modules/render/*.cpp" "modules/render/*.h")
add_library(render)
target_sources(render PRIVATE ${MOD_RENDER_SRC})
target_link_libraries(render vulkan ui scene config geometry algebra event)

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

# Module wavefront
# ================================
file(GLOB MOD_WAVEFRONT_SRC "modules/wavefront/*.cpp")
add_library(wavefront)
target_sources(wavefront
  PUBLIC
    FILE_SET CXX_MODULES FILES
    ${MOD_WAVEFRONT_SRC}
)

# Module scene
# ================================
file(GLOB MOD_SCENE_SRC "modules/scene/*.cpp" "modules/scene/*.h")
add_library(scene)
target_sources(scene PRIVATE ${MOD_SCENE_SRC})
target_link_libraries(scene config image algebra geometry)

# Module event
# ================================
#add_library(event)
#target_sources(event
#  PUBLIC
#    FILE_SET CXX_MODULES FILES
#    modules/event/interface.cpp
#    modules/event/async.cpp
#    modules/event/context.cpp
#    modules/event/timer.cpp
#)
#target_link_libraries(event config)

# Module io
# ================================
#add_library(io)
#target_sources(io
#  PUBLIC
#    FILE_SET CXX_MODULES FILES
#    modules/io/interface.cpp
#    modules/io/file.cpp
#)
#target_link_libraries(io )

# Main apps subdirectories
# ================================
add_executable(barevk)
file(GLOB_RECURSE EXE_BAREVK_SRC "apps/bare/*.cpp" "apps/bare/*.h")
target_sources(barevk PRIVATE ${EXE_BAREVK_SRC})
target_link_libraries(barevk PUBLIC 
    log 
    algebra 
    scene 
    config 
    render 
    glfw3
    Vulkan::Vulkan 
    uv
)

add_executable(qtvk)
set_target_properties(qtvk PROPERTIES AUTOMOC TRUE)
file(GLOB_RECURSE EXE_QTVK_SRC "apps/qt/*.cpp" "apps/qt/*.h")
target_sources(qtvk PRIVATE ${EXE_QTVK_SRC})
target_link_libraries(qtvk PUBLIC
 Vulkan::Vulkan 
 render 
    Qt::Core
    Qt::Gui
    Qt6::GuiPrivate
    Qt::Quick
    Qt::QuickWidgets
    Qt::QuickControls2
    Qt::Qml
    log 
    algebra 
    scene 
    config 
    glfw3
    uv
)

# Test subdirectories
# ================================
add_subdirectory(test/wavefront)
target_link_libraries(obj_test wavefront)

add_subdirectory(test/color)
target_link_libraries(color image)
