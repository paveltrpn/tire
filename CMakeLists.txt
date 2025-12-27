cmake_minimum_required(VERSION 4.2)

set(USE_CLANG ON)
#set(USE_SYSTEM_GCC ON)
#set(USE_LATEST_GCC ON)

# Choose compiler toolchain.
if (USE_CLANG)
    message( "USE CLANG TOOLCHAIN")
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)

    # Use clang stdlib - libc++
    add_compile_options(-stdlib=libc++)
    add_link_options(-stdlib=libc++)
elseif(USE_SYSTEM_GCC)
    message( "USE SYSTEM GCC")
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
elseif(USE_LATEST_GCC)
    message( "USE LATEST GCC")
    set(CMAKE_C_COMPILER $ENV{GCC_LATEST}/bin/gcc-15.1)
    set(CMAKE_CXX_COMPILER $ENV{GCC_LATEST}/bin/g++-15.1)
endif()

# Silence some deprecated declaration warnings from nlohmann json and Qt
add_compile_options(-Wno-deprecated-declarations)

# Set latest c++ standart.
add_compile_options(-std=c++26)
set(CMAKE_CXX_STANDARD 26)

# Build type.
set(CMAKE_BUILD_TYPE Debug)

project (tiny_render)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON )
set(SURFACE_X11 CACHE BOOL "Enable X11 surface backend.")
set(SURFACE_WAYLAND ON CACHE BOOL "Enable wayland surface backend.")

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

# ================================
# Module log
# ================================
file(GLOB MOD_LOG_SRC "modules/log/*.cpp")
add_library(log)
target_sources(log
    PUBLIC FILE_SET CXX_MODULES FILES
    ${MOD_LOG_SRC}
)

# ================================
# Module algebra
# ================================
file(GLOB MOD_ALGEBRA_SRC "modules/algebra/*.cpp")
add_library(algebra)
target_sources(algebra
    PUBLIC FILE_SET CXX_MODULES FILES
    ${MOD_ALGEBRA_SRC}
)
set_target_properties(algebra PROPERTIES LINKER_LANGUAGE CXX)

# ================================
# Module config
# ================================
file(GLOB MOD_CONFIG_SRC "modules/config/*.cpp")
add_library(config)
target_sources(config
    PUBLIC FILE_SET CXX_MODULES FILES
    ${MOD_CONFIG_SRC}
)
target_link_libraries(config log)

# ================================
# Module context
# ================================
file(GLOB_RECURSE MOD_CONTEXT_SRC "modules/context/*.cpp" "modules/context/*.h")
add_library(context)
target_sources(context PRIVATE ${MOD_CONTEXT_SRC})
target_link_libraries(context vulkan config)

# ================================
# Module ui
# ================================
file(GLOB MOD_UI_SRC "modules/ui/*.cpp")
add_library(ui )
target_sources(ui
    PUBLIC FILE_SET CXX_MODULES FILES
    ${MOD_UI_SRC}
)

# ================================
# Module io
# ================================
add_library(io)
file(GLOB MOD_IO_SRC "modules/io/*.cpp")
target_sources(io
    PUBLIC FILE_SET CXX_MODULES FILES
    ${MOD_IO_SRC}
)
target_link_libraries(io uv log)

# ================================
# Module render
# ================================
file(GLOB_RECURSE MOD_RENDER_SRC "modules/render/*.cpp")
add_library(render)
target_sources(render
    PUBLIC FILE_SET CXX_MODULES FILES
    ${MOD_RENDER_SRC}
)
target_link_libraries(render vulkan context ui scene config geometry algebra io)

# ================================
# Module image
# ================================
file(GLOB MOD_IMAGE_SRC "modules/image/*.cpp")
add_library(image)
target_sources(image
    PUBLIC FILE_SET CXX_MODULES FILES
    ${MOD_IMAGE_SRC}
)
target_link_libraries(image algebra log)

# ================================
# Module geometry
# ================================
file(GLOB MOD_GEOMETRY_SRC "modules/geometry/*.cpp" "modules/geometry/*.h")
add_library(geometry)
target_sources(geometry PRIVATE ${MOD_GEOMETRY_SRC})
target_link_libraries(geometry algebra)

# ================================
# Module scene
# ================================
file(GLOB MOD_SCENE_SRC "modules/scene/*.cpp")
add_library(scene)
target_sources(scene
    PUBLIC FILE_SET CXX_MODULES FILES
    ${MOD_SCENE_SRC}
)
target_link_libraries(scene config image algebra geometry)

# ================================
# Main apps subdirectories
# ================================
add_executable(barevk)
file(GLOB_RECURSE EXE_BAREVK_SRC "apps/bare/*.cpp" "apps/bare/*.h")
target_sources(barevk PRIVATE ${EXE_BAREVK_SRC})
target_link_libraries(barevk PUBLIC
    context
    log
    algebra
    scene
    config
    render
    glfw3
    Vulkan::Vulkan
    uv
)

set(BUILD_QT OFF)
if (BUILD_QT)
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

    add_executable(qtvk)
    set_target_properties(qtvk PROPERTIES AUTOMOC TRUE)
    file(GLOB_RECURSE EXE_QTVK_SRC "apps/qt/*.cpp" "apps/qt/*.h")
    target_sources(qtvk PRIVATE ${EXE_QTVK_SRC})
    target_link_libraries(qtvk PUBLIC
        stdc++
        Vulkan::Vulkan
        render
        Qt::Core
        Qt::Gui
        Qt::GuiPrivate
        Qt::Quick
        Qt::QuickWidgets
        Qt::QuickControls2
        Qt::Qml
        context
        log
        algebra
        scene
        config
        glfw3
        uv
    )
    # target_compile_options(qtvk PUBLIC
    #     -fmodule-file=render=CMakeFiles/render.dir/render.pcm
    #     -fmodule-file=render:render=CMakeFiles/render.dir/render-render.pcm
    #     -fmodule-file=render:timer=CMakeFiles/render.dir/render-timer.pcm
    #     -fmodule-file=render:scenevk=CMakeFiles/render.dir/render-scenevk.pcm
    #     -fmodule-file=render:program=CMakeFiles/render.dir/render-program.pcm
    #     -fmodule-file=render:pipeline=CMakeFiles/render.dir/render-pipeline.pcm
    #     -fmodule-file=render:pipeline_test_box=CMakeFiles/render.dir/render-pipeline_test_box.pcm
    #     -fmodule-file=render:test_box_shader=CMakeFiles/render.dir/render-test_box_shader.pcm
    #     -fmodule-file=render:pipeline_vertex_buffer=CMakeFiles/render.dir/render-pipeline_vertex_buffer.pcm
    #     -fmodule-file=render:plain_image=CMakeFiles/render.dir/render-plain_image.pcm
    #     -fmodule-file=render:texture_image=CMakeFiles/render.dir/render-texture_image.pcm
    #     -fmodule-file=render:plain_buffer=CMakeFiles/render.dir/render-plain_buffer.pcm
    #     -fmodule-file=render:vertex_buffer=CMakeFiles/render.dir/render-vertex_buffer.pcm
    # )
endif()

# Test subdirectories
# ================================
add_subdirectory(test)
