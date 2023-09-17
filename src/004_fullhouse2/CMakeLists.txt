cmake_minimum_required(VERSION 3.20)

set(SOURCES main.cpp
            apputils.cpp
            mesh.cpp
            bitmap.cpp
            base64.cpp
            camera.cpp
            timing.cpp
            screen_text.cpp)

add_executable(004_fullhouse2 ${SOURCES})

