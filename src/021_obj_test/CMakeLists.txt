
cmake_minimum_required(VERSION 3.0)

project (021_obj_test)

set(CMAKE_CXX_COMPILER g++-12)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c++20")

set(LIBS -lstdc++ -lm)

include_directories("../../extern/glfw/include")
include_directories("../../extern/fmt/include") 
include_directories("../../internal")

link_directories("../../extern/glfw/build/src")
link_directories("../../extern/fmt/build")
link_directories("../../internal/wavefront_obj")

set(WAVEFRONTOBJ_SRC_DIR ../../internal/wavefront_obj)

set(SOURCES main.cpp
    ${WAVEFRONTOBJ_SRC_DIR}/obj.cpp
    ${WAVEFRONTOBJ_SRC_DIR}/mtl.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "")

add_executable(main ${SOURCES} )
target_link_libraries(main ${LIBS})
