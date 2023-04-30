
cmake_minimum_required(VERSION 3.0)

project (vk_basic)

include_directories("../../extern/glfw/include")
include_directories("../../extern/fmt/include") 

link_directories("../../extern/glfw/build/src")
link_directories("../../extern/fmt/build")

set(CMAKE_CXX_COMPILER g++-12)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c++20")

set(LIBS -lstdc++ -lfmt -lglfw3 -lm -lvulkan)

set(SOURCES main.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "")

add_executable(main ${SOURCES} )
target_link_libraries(main ${LIBS})
