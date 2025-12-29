cmake_minimum_required(VERSION 4.2)

add_executable(color
    color/main.cpp)
target_link_libraries(color log image)
