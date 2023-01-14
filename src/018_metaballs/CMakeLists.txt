cmake_minimum_required(VERSION 3.0)

project (018_metaballs)

set(CMAKE_CXX_COMPILER g++-12)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
-Wextra -Wpedantic -Wno-unused-parameter -std=c++20")

set(LIBS -lstdc++ -lfmt -lGL -lGLU -lGLEW -lglfw3 -ltga -llibjpeg)

set(IMGUI_SRC ../../extern/imgui)
set(IMGUI_BACK_SRC ../../extern/imgui/backends)

set(SOURCES main.cpp
            apputils.cpp
            bitmap.cpp
            camera.cpp)

set(IMGUI   ${IMGUI_SRC}/imgui_tables.cpp 
            ${IMGUI_SRC}/imgui_widgets.cpp
            ${IMGUI_BACK_SRC}/imgui_impl_glfw.cpp 
            ${IMGUI_SRC}/imgui.cpp
            ${IMGUI_SRC}/imgui_draw.cpp
            ${IMGUI_BACK_SRC}/imgui_impl_opengl2.cpp)

include_directories("../../extern/fmt/include") 
include_directories("../../extern/glfw/include")
include_directories("../../extern/glew-2.2.0/include")
include_directories("../../extern/imgui")
include_directories("../../extern/libjpeg-cmake")
include_directories("../../extern/libtga/include")
include_directories("../../extern/glm")

link_directories("../../extern/libjpeg-cmake/build")
link_directories("../../extern/libtga/build/src")
link_directories("../../extern/glew-2.2.0/lib")
link_directories("../../extern/glfw/build/src")
link_directories("../../extern/fmt/build")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../")

add_executable(main ${SOURCES} ${IMGUI})
target_link_libraries(main ${LIBS})
