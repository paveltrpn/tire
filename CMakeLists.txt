cmake_minimum_required(VERSION 4.2)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

project(tire)

include_directories("modules")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(CXX_STANDART -std=c++26)

if (DEFINED ENV{VULKAN_SDK} AND NOT "$ENV{VULKAN_SDK}" STREQUAL "")
    message( "VULKAN_SDK is set to $ENV{VULKAN_SDK}")
    include_directories($ENV{VULKAN_SDK}/include)
else()
    message(SEND_ERROR "VULKAN_SDK is not set")
endif()

if (DEFINED ENV{VMA_DIR} AND NOT "$ENV{VMA_DIR}" STREQUAL "")
    message( "VMA_DIR is set to $ENV{VMA_DIR}")
    include_directories($ENV{VMA_DIR}/include)
else()
    message(SEND_ERROR "VMA_DIR is not set")
endif()

find_package(glslang 15.0 REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(nlohmann_json 3.11 REQUIRED)

# ================================
# Module config
# ================================
file(GLOB MOD_MESH_SRC "modules/mesh/*.cpp" "modules/mesh/*.h")
add_library(mesh ${MOD_MESH_SRC})
target_link_libraries(mesh log)
target_compile_options(mesh PRIVATE "${CXX_STANDART}")
target_link_libraries(mesh
   algebra
)

# ================================
# Module config
# ================================
file(GLOB MOD_CONFIG_SRC "modules/config/*.cpp" "modules/config/*.h")
add_library(config ${MOD_CONFIG_SRC})
target_link_libraries(config log)
target_compile_options(config PRIVATE "${CXX_STANDART}")

# ================================
# Module context
# ================================
file(GLOB_RECURSE MOD_CONTEXT_SRC "modules/context/*.cpp" "src/context/*.h")
add_library(context ${MOD_CONTEXT_SRC})
target_link_libraries(context vulkan config image)
target_compile_options(context PRIVATE "${CXX_STANDART}")

# ================================
# Module r_ui
# ================================
file(GLOB MOD_R_UI_SRC "modules/ui/*.cpp" "modules/ui/*.h")
add_library(r_ui ${MOD_R_UI_SRC})
target_link_libraries(r_ui log algebra image)
target_compile_options(r_ui PRIVATE "${CXX_STANDART}")

# ================================
# Module render
# ================================
file(GLOB_RECURSE MOD_RENDER_SRC "modules/render/*.cpp" "modules/render/*.h")
add_library(render ${MOD_RENDER_SRC})
target_link_libraries(render vulkan context r_ui config algebra)
target_compile_options(render PRIVATE "${CXX_STANDART}")

# ================================
# Module algebra
# ================================
file(GLOB_RECURSE MOD_ALGEBRA_SRC "modules/algebra/*.h")
add_library(algebra ${MOD_ALGEBRA_SRC})
target_compile_options(algebra PRIVATE "${CXX_STANDART}")
set_target_properties(algebra PROPERTIES LINKER_LANGUAGE CXX)

# ================================
# Module log
# ================================
file(GLOB_RECURSE MOD_LOG_SRC "modules/log/*.cpp" "modules/log/*.h")
add_library(log ${MOD_LOG_SRC})
target_compile_options(log PRIVATE "${CXX_STANDART}")
target_link_libraries(log
   algebra
)

# ================================
# Module image
# ================================
file(GLOB_RECURSE MOD_IMAGE_SRC "modules/image/*.cpp" "modules/image/*.h")
add_library(image ${MOD_IMAGE_SRC})
target_compile_options(image PRIVATE "${CXX_STANDART}")
target_link_libraries(image
    log
)

# ================================
# Module event
# ================================
file(GLOB_RECURSE MOD_EVENT_SRC "modules/event/*.cpp" "modules/event/*.h")
add_library(event ${MOD_EVENT_SRC})
target_compile_options(event PRIVATE "${CXX_STANDART}")
target_link_libraries(event
    log
)

# ================================
# tire executable
# ================================
set(TIRE_EXE tire)
add_executable(${TIRE_EXE})
file(GLOB_RECURSE EXE_R_SRC "tire/*.cpp" "tire/*.h")
target_sources(${TIRE_EXE} PRIVATE ${EXE_R_SRC})
target_compile_options(${TIRE_EXE} PRIVATE "${CXX_STANDART}")
target_link_libraries(${TIRE_EXE}
    event
    context
    log
    algebra
    config
    render
    glfw
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
    
