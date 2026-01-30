cmake_minimum_required(VERSION 4.2)

set(CMAKE_C_COMPILER $ENV{GCC_LATEST}/bin/gcc-15.1)
set(CMAKE_CXX_COMPILER $ENV{GCC_LATEST}/bin/g++-15.1)
add_compile_options(-std=c++26)
set(CMAKE_BUILD_TYPE Debug)

project(tire_ed)

include_directories("src")

set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(CMAKE_CXX_STANDARD 26)

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

if (DEFINED ENV{vsgqt_DIR} AND NOT "$ENV{vsgqt_DIR}" STREQUAL "")
    message( "vsgqt_DIR is set to $ENV{vsgqt_DIR}")
    include_directories($ENV{vsgqt_DIR}/include)
    link_directories($ENV{vsgqt_DIR}/lib)
else()
    message(SEND_ERROR "vsgqt_DIR is not set")
endif()

if (DEFINED ENV{vsgXchange_DIR} AND NOT "$ENV{vsgXchange_DIR}" STREQUAL "")
    message( "vsgXchange_DIR is set to $ENV{vsgXchange_DIR}")
    include_directories($ENV{vsgXchange_DIR}/include)
    link_directories($ENV{vsgXchange_DIR}/lib)
else()
    message(SEND_ERROR "vsgXchange_DIR is not set")
endif()

if (DEFINED ENV{glslang_DIR} AND NOT "$ENV{glslang_DIR}" STREQUAL "")
    message( "glslang_DIR is set to $ENV{glslang_DIR}")
    include_directories($ENV{glslang_DIR}/include)
    link_directories($ENV{glslang_DIR}/lib)
else()
    message(SEND_ERROR "glslang_DIR is not set")
endif()

include_directories("src/modules")

# ================================
# Module box
# ================================
file(GLOB MOD_BOX_SRC "src/modules/box/*.cpp" "src/modules/box/*.h")
add_library(box)
target_sources(box PRIVATE ${MOD_BOX_SRC})
set_target_properties(box PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(box vsg)

set(QT_VERSION 6.10)
list(APPEND CMAKE_PREFIX_PATH $ENV{LOCAL_LATEST_QT})
set(QML_IMPORT_PATH  $ENV{LOCAL_LATEST_QT}/qml client_vk/qml client_vk/qml/info CACHE STRING "Extra QML Import Paths for Qt Creator" FORCE)
find_package(Qt6 ${QT_VERSION} COMPONENTS
    Core
    Gui
    GuiPrivate
    Quick
    QuickWidgets
    QuickControls2
    Qml
    Charts
)

if (vsgXchange_FOUND)
    target_compile_definitions(${PROJECT_NAME} PRIVATE vsgXchange_FOUND)
    target_link_libraries(${PROJECT_NAME} vsgXchange::vsgXchange)
endif()

file(GLOB MAIN_APP "src/*.cpp" "src/*.h")
file(GLOB UI_SRC "src/ui/*.cpp" "src/ui/*.h")
file(GLOB TIRED_SRC "src/tired/*.cpp" "src/tired/*.h")
add_executable(${PROJECT_NAME} ${MAIN_APP}  ${UI_SRC} ${TIRED_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)

target_link_libraries(${PROJECT_NAME}
    xcb
    vsgQt
    vsg
    vsgXchange
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
    Qt6::GuiPrivate
    Qt::Quick
    Qt::QuickWidgets
    Qt::QuickControls2
    Qt::Qml
    box
)

    
