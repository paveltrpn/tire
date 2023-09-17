cmake_minimum_required(VERSION 3.20)

set(SOURCES main.cpp
            mainApp.cpp
            mesh.cpp
            bitmap.cpp
            bitmap_text.cpp
            camera.cpp)

add_executable(001_fullhouse ${SOURCES})

