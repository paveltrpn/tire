cmake_minimum_required(VERSION 3.31)

add_executable(obj_test
    wavefront/main.cpp)
target_link_libraries(obj_test log wavefront)

add_executable(color
    color/main.cpp)
target_link_libraries(color log image)
