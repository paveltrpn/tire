cmake_minimum_required(VERSION 4.2)

set(USE_LATEST_GCC ON)
# set(USE_SYSTEM_GCC ON)

# Choose compiler toolchain.
if(USE_SYSTEM_GCC)
    message( "USE SYSTEM GCC")
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
elseif(USE_LATEST_GCC)
    message( "USE LATEST GCC")
    set(CMAKE_C_COMPILER $ENV{GCC_LATEST}/bin/gcc)
    set(CMAKE_CXX_COMPILER $ENV{GCC_LATEST}/bin/g++)
endif()

project(tire_ed)

include_directories("src")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

if (DEFINED ENV{VULKAN_SDK} AND NOT "$ENV{VULKAN_SDK}" STREQUAL "")
    message( "VULKAN_SDK is set to $ENV{VULKAN_SDK}")
    include_directories($ENV{VULKAN_SDK}/include)
else()
    message(SEND_ERROR "VULKAN_SDK is not set")
endif()

if (DEFINED ENV{vsg_DIR} AND NOT "$ENV{vsg_DIR}" STREQUAL "")
    message( "vsg_DIR is set to $ENV{vsg_DIR}")
    include_directories($ENV{vsg_DIR}/include)
    link_directories($ENV{vsg_DIR}/lib)
else()
    message(SEND_ERROR "vsg_DIR is not set")
endif()

if (DEFINED ENV{glslang_DIR} AND NOT "$ENV{glslang_DIR}" STREQUAL "")
    message( "glslang_DIR is set to $ENV{glslang_DIR}")
    include_directories($ENV{glslang_DIR}/include)
    link_directories($ENV{glslang_DIR}/lib)
else()
    message(SEND_ERROR "glslang_DIR is not set")
endif()

include_directories("src")

set(QT_VERSION 6.10)
list(APPEND CMAKE_PREFIX_PATH $ENV{LOCAL_LATEST_QT})
# set(QML_IMPORT_PATH  $ENV{LOCAL_LATEST_QT}/lib/cmake CACHE STRING "Extra QML Import Paths for Qt Creator" FORCE)
set(QML_IMPORT_PATH /mnt/main/sources_build/qt6_10/qml CACHE STRING "Extra QML Import Paths for Qt Creator" FORCE)
find_package(Qt6 ${QT_VERSION} COMPONENTS
    Core
    Gui
    Qml
    Quick
    QuickWidgets
    QuickControls2
    QuickLayouts
)

# ================================
# Module config
# ================================
file(GLOB MOD_CONFIG_SRC "src/config/*.cpp")
add_library(config)
target_sources(config
    PUBLIC FILE_SET CXX_MODULES FILES
    ${MOD_CONFIG_SRC}
)
target_link_libraries(config log)

# ================================
# Module vma_usage
# ================================
file(
    GLOB_RECURSE
    MOD_VMA_USAGE_SRC
    "src/vma_usage/*.cpp" "src/vma_usage/*.h"
)
add_library(vma_usage)
target_sources(
    vma_usage
    PRIVATE
    ${MOD_VMA_USAGE_SRC}
)

# ================================
# Module context
# ================================
file(GLOB_RECURSE MOD_CONTEXT_SRC "src/context/*.cpp" )
add_library(context)
target_sources(context
    PUBLIC FILE_SET CXX_MODULES FILES
    ${MOD_CONTEXT_SRC})
target_link_libraries(context vulkan config image vma_usage)

# ================================
# Module r_ui
# ================================
file(GLOB MOD_UI_SRC "src/r_ui/*.cpp")
add_library(ui )
target_sources(ui
    PUBLIC FILE_SET CXX_MODULES FILES
    ${MOD_UI_SRC}
)
target_link_libraries(r_ui log algebra image event)

# ================================
# Module render
# ================================
file(GLOB_RECURSE MOD_RENDER_SRC "src/render/*.cpp")
add_library(render)
target_sources(render
    PUBLIC FILE_SET CXX_MODULES FILES
    ${MOD_RENDER_SRC}
)
target_link_libraries(render vulkan context ui scene config geometry algebra vma_usage event)

# ================================
# Module scene
# ================================
file(GLOB MOD_SCENE_SRC "src/scene/*.cpp")
add_library(scene)
target_sources(scene
    PUBLIC FILE_SET CXX_MODULES FILES
    ${MOD_SCENE_SRC}
)
target_link_libraries(scene config image algebra geometry generator event)

# ================================
# Module generator
# ================================
file(GLOB MOD_GENERATOR_SRC "src/generator/*.cpp")
add_library(generator)
target_sources(generator
    PUBLIC FILE_SET CXX_MODULES FILES
    ${MOD_GENERATOR_SRC}
)
target_link_libraries(generator)

# ================================
# Module event
# ================================
file(GLOB MOD_EVENT_SRC "src/event/*.cpp")
add_library(event)
target_sources(event
    PUBLIC FILE_SET CXX_MODULES FILES
    ${MOD_EVENT_SRC}
)
target_link_libraries(event)

# ================================
# Module lib/fast_obj
# ================================
add_subdirectory(lib/fast_obj)

# ================================
# Module lib/libtga
# ================================
add_subdirectory(lib/libtga)

# ================================
# Module algebra
# ================================
file(GLOB_RECURSE MOD_ALGEBRA_SRC "src/algebra/*.cpp" "src/algebra/*.h")
add_library(algebra ${MOD_ALGEBRA_SRC})
set_target_properties(algebra PROPERTIES AUTOMOC TRUE)
target_compile_options(algebra PRIVATE -freflection -std=c++26)

# ================================
# Module log
# ================================
file(GLOB_RECURSE MOD_LOG_SRC "src/log/*.cpp" "src/log/*.h")
add_library(log ${MOD_LOG_SRC})
set_target_properties(log PROPERTIES AUTOMOC TRUE)
target_compile_options(log PRIVATE -freflection -std=c++26)
target_link_libraries(log
   algebra
)

# ================================
# Module vk
# ================================
file(GLOB_RECURSE MOD_VK_SRC "src/vk/*.cpp" "src/vk/*.h")
add_library(vk ${MOD_VK_SRC})
set_target_properties(vk PROPERTIES AUTOMOC TRUE)
target_compile_options(vk PRIVATE -freflection -std=c++26)
target_link_libraries(vk
   vulkan
   log
)

# ================================
# Module image
# ================================
file(GLOB_RECURSE MOD_IMAGE_SRC "src/image/*.cpp" "src/image/*.h")
add_library(image ${MOD_IMAGE_SRC})
set_target_properties(image PROPERTIES AUTOMOC TRUE)
target_compile_options(image PRIVATE -freflection -std=c++26)
target_link_libraries(image
    log
    libtga
)

# ================================
# Module tired
# ================================
file(GLOB_RECURSE MOD_TIRED_SRC "src/tired/*.cpp" "src/tired/*.h")
add_library(tired ${MOD_TIRED_SRC})
set_target_properties(tired PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(tired PROPERTIES AUTOMOC TRUE)
target_compile_options(tired PRIVATE -freflection -std=c++26)
target_link_libraries(tired
    vsg
    vulkan
    GenericCodeGen
    glslang
    glslang-default-resource-limits
    MachineIndependent
    OSDependent
    SPIRV-Tools-opt
    SPIRV-Tools
    SPIRV-Tools-diff
    SPIRV-Tools-link
    SPIRV-Tools-lint
    SPIRV-Tools-reduce
    SPIRV
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::Qml
    Qt::QuickWidgets
    Qt::QuickControls2
    Qt::QuickLayouts
    vk
    log
    image
    fast_obj
)

# ================================
# Module tired_ui
# ================================
file(GLOB_RECURSE MOD_TIRED_UI_SRC "src/ui/*.cpp" "src/ui/*.h")
add_library(tired_ui ${MOD_TIRED_UI_SRC})
set_target_properties(tired_ui PROPERTIES AUTOMOC TRUE)
target_compile_options(tired_ui PRIVATE -freflection -std=c++26)
target_link_libraries(tired_ui
    vsg
    vk
    image
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::Qml
    Qt::QuickWidgets
    Qt::QuickControls2
    Qt::QuickLayouts
)

# ================================
# Module qt_window
# ================================
file(GLOB MOD_QT_WINDOW_SRC "src/qt_window/*.cpp" "src/qt_window/*.h")
add_library(qt_window ${MOD_QT_WINDOW_SRC})
set_target_properties(qt_window PROPERTIES AUTOMOC TRUE)
target_compile_options(qt_window PRIVATE -freflection -std=c++26)
target_link_libraries(qt_window
    vsg
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::Qml
    Qt::QuickWidgets
    Qt::QuickControls2
    Qt::QuickLayouts
)

# ================================
# tire editor executable
# ================================
set(TIRE_EDITOR_EXE ed)
add_executable(${TIRE_EDITOR_EXE}
    "src/ed/main.cpp"
    "src/ed/tiredapplication.cpp"
    "src/ed/tiredapplication.h"
)
set_target_properties(${TIRE_EDITOR_EXE} PROPERTIES AUTOMOC TRUE)
target_compile_options(${TIRE_EDITOR_EXE} PRIVATE -freflection -std=c++26)
target_link_libraries(${TIRE_EDITOR_EXE}
    xcb
    vsg
    vulkan
    GenericCodeGen
    glslang
    glslang-default-resource-limits
    MachineIndependent
    OSDependent
    SPIRV-Tools-opt
    SPIRV-Tools
    SPIRV-Tools-diff
    SPIRV-Tools-link
    SPIRV-Tools-lint
    SPIRV-Tools-reduce
    SPIRV
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::Qml
    Qt::QuickWidgets
    Qt::QuickControls2
    Qt::QuickLayouts
    tired
    tired_ui
    qt_window
)

# ================================
# tire executable
# ================================
set(TIRE_EXE r)
add_executable(${TIRE_EXE}
    "src/r/main.cpp"
)
set_target_properties(${TIRE_EXE} PROPERTIES AUTOMOC TRUE)
target_compile_options(${TIRE_EXE} PRIVATE -freflection -std=c++26)
target_link_libraries(${TIRE_EXE}
    xcb
    vulkan
    GenericCodeGen
    glslang
    glslang-default-resource-limits
    MachineIndependent
    OSDependent
    SPIRV-Tools-opt
    SPIRV-Tools
    SPIRV-Tools-diff
    SPIRV-Tools-link
    SPIRV-Tools-lint
    SPIRV-Tools-reduce
    SPIRV
)

    
