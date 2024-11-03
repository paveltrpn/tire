cmake_minimum_required(VERSION 3.28)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
add_compile_options(-stdlib=libc++)
add_link_options(-stdlib=libc++)

project (tiny_render)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(CMAKE_BUILD_TYPE Debug)

include_directories("3rdparty")
include_directories("modules")

add_library(log
    modules/log/log.h
    modules/log/log.cpp
)
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
    modules/render/rendergl.cpp
    modules/render/rendergl.h
    modules/render/rendervk.cpp
    modules/render/rendervk_printinfo.cpp
    modules/render/rendervk.h
    modules/render/node_list.cpp
    modules/render/node_list.h
)
target_link_libraries(render camera gl config geometry algebra)

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

add_library(image)
target_sources(image
  PUBLIC
    FILE_SET CXX_MODULES FILES
    modules/image/interface.cpp
    modules/image/bmp.cpp
    modules/image/tga.cpp
    modules/image/canvas.cpp
)

add_library(camera
    modules/camera/camera.h
    modules/camera/camera.cpp
)
target_link_libraries(camera algebra)

add_library(gl
    modules/gl/functions.cpp
    modules/gl/functions.h
    modules/gl/shader.cpp
    modules/gl/shader.h
    modules/gl/shader_sources.cpp
    modules/gl/shader_sources.h
)
target_link_libraries(gl algebra)

add_library(vk
    modules/vk/vulkan_context.cpp
    modules/vk/vulkan_context.h
)
target_link_libraries(vk)

add_library(geometry
    modules/geometry/node.cpp
    modules/geometry/node.h
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

add_subdirectory(test/window)
target_link_libraries(gl_test log algebra camera gl config render X11 GL GLU vulkan)
target_link_libraries(vk_test log algebra camera vk config render X11 GL GLU vulkan)

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

