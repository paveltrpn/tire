cmake_minimum_required(VERSION 3.0)

project (001_fullhouse)

set(LIBJPEG_DIR "../../extern/libjpeg-cmake")
add_subdirectory(${LIBJPEG_DIR} ../${LIBJPEG_DIR}/build)
include_directories(${LIBJPEG_DIR})

set(LIBTGA_DIR "../../extern/libtga")
add_subdirectory(${LIBTGA_DIR} ../${LIBTGA_DIR}/build)
include_directories(${LIBTGA_DIR}/include)

set(GLFW_DIR "../../extern/glfw")
add_subdirectory(${GLFW_DIR} ../${GLFW_DIR}/build)
include_directories(${GLFW_DIR})

set(GLEW_DIR "../../extern/glew-cmake")
add_subdirectory(${GLEW_DIR} ../${GLEW_DIR}/build)
include_directories(${GLEW_DIR}/include)
link_directories(${GLEW_DIR}/build/lib)

set(CMAKE_CXX_COMPILER g++-13.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall \
-Wextra -Wpedantic -Wno-unused-parameter -std=c++20")

set(LIBS -lstdc++ -lGL -lGLU -lGLEW)

set(SOURCES main.cpp
            apputils.cpp
            mesh.cpp
            camera.cpp
            timing.cpp
            screen_text.cpp
            matbase.cpp
            bitmap.cpp)

set(ALGEBRA_DIR "../../internal/algebra")
add_subdirectory(${ALGEBRA_DIR} "./algebra")
include_directories(${ALGEBRA_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ".")

add_executable(main ${SOURCES})

target_link_libraries(main algebra libjpeg libtga GLEW glfw ${LIBS})
