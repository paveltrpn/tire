CMAKE_MINIMUM_REQUIRED(VERSION 4.2)

project(fast_obj)

add_library(fast_obj INTERFACE)
target_include_directories(fast_obj INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_library(fast_obj_lib STATIC fast_obj.c)
target_include_directories(fast_obj_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
