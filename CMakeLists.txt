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

include_directories("3rdparty")
include_directories("modules")

add_subdirectory(modules/toy_std)

add_library(config)
target_sources(config
  PUBLIC
    FILE_SET CXX_MODULES FILES
        modules/config/interface.cpp
        modules/config/config.cpp
)

add_library(render)
target_sources(render
  PUBLIC
    FILE_SET CXX_MODULES FILES
        modules/render/interface.cpp
        modules/render/render.cpp
        modules/render/render_def.cpp
        modules/render/rendergl.cpp
        modules/render/rendergl_def.cpp
        modules/render/rendervk.cpp
        modules/render/rendervk_def.cpp
)
target_link_libraries(render camera opengl_types config assets toy_std)

set(QT_VERSION 6.8)
list(APPEND CMAKE_PREFIX_PATH $ENV{LOCAL_QT_DIR})
find_package(Qt6 ${QT_VERSION} COMPONENTS
    Core
    Gui
    Quick
    QuickWidgets
    QuickControls2
    Qml
    OpenGL)

add_library(qt_render
        modules/qt_render/qt_render.h
        modules/qt_render/qt_render.cpp
        modules/qt_render/render_window.h
        modules/qt_render/render_window.cpp
)
set_target_properties(qt_render PROPERTIES AUTOMOC TRUE)
target_link_libraries(qt_render PUBLIC
    Qt::Core
    Qt::Quick
    Qt::QuickWidgets
    Qt::QuickControls2
    Qt::Qml
    Qt::OpenGL)

add_library(assets)
target_sources(assets
  PUBLIC
    FILE_SET CXX_MODULES FILES
        modules/assets/interface.cpp
        modules/assets/shader_sources.cpp
)
target_link_libraries(assets toy_std geometry)

add_library(image)
target_sources(image
  PUBLIC
    FILE_SET CXX_MODULES FILES
    modules/image/interface.cpp
    modules/image/bmp.cpp
    modules/image/tga.cpp
    modules/image/canvas.cpp
)

add_library(camera)
target_sources(camera
    PUBLIC
      FILE_SET CXX_MODULES FILES
        modules/camera/interface.cpp
        modules/camera/camera.cpp
)
target_link_libraries(camera toy_std)

add_library(opengl_types)
target_sources(opengl_types
  PUBLIC
      FILE_SET CXX_MODULES FILES
        modules/opengl_types/interface.cpp
        modules/opengl_types/functions.cpp
        modules/opengl_types/shader.cpp
)
target_link_libraries(opengl_types toy_std)

add_library(vulkan_types)
target_sources(vulkan_types
  PUBLIC
      FILE_SET CXX_MODULES FILES
        modules/vulkan_types/interface.cpp
        modules/vulkan_types/vulkan_context.cpp
)
target_link_libraries(vulkan_types)

add_library(geometry)
target_sources(geometry
  PUBLIC
      FILE_SET CXX_MODULES FILES
        modules/geometry/interface.cpp
        modules/geometry/node.cpp
        modules/geometry/transformations.cpp
        modules/geometry/point.cpp
        modules/geometry/normal.cpp
        modules/geometry/wavefront.cpp
        modules/geometry/polytope.cpp
)
target_link_libraries(geometry toy_std)

add_subdirectory(test/window)
target_link_libraries(gl camera opengl_types config render assets X11 GL GLU vulkan)
target_link_libraries(vk camera vulkan_types config render assets X11 GL GLU vulkan)

add_subdirectory(test/wavefront)
target_link_libraries(wavefront geometry)

add_subdirectory(test/qt)
target_link_libraries(qt PUBLIC
    qt_render
    Qt::Core
    Qt::Quick
    Qt::QuickWidgets
    Qt::QuickControls2
    Qt::Qml
    Qt::OpenGL)

