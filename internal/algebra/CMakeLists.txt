set(ALGEBRA_SRC mtrx2.cpp
                mtrx3.cpp
                mtrx4.cpp
                vec2.cpp
                vec3.cpp
                vec4.cpp
                qtnn.cpp
                plane.cpp)

add_library(algebra OBJECT ${ALGEBRA_SRC})

