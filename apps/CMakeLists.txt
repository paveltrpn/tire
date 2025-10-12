cmake_minimum_required(VERSION 3.30)

add_executable(barevk
    bare/vk.cpp
    bare/bare.cpp
    bare/bare.h
    bare/command_buffer.cpp
    bare/command_buffer.h
)

add_executable(qtvk
    qt/vk.cpp
    qt/qt.cpp
    qt/qt.h
)

