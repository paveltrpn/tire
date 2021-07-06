# Demos

Сборник простых графических программ.  

Библиотеки, необходимые для сборки:  

- **SDL2** (sudo apt-get install libsdl2-dev)  
- **FreeGLUT** (sudo apt-get install freeglut3-dev)  
- **GLFW** (sudo apt-get install libglfw3 libglfw3-dev)
- **GLEW** (sudo apt-get install libglew2.0 libglew-dev)
- **libjpeg** (sudo apt-get install libjpeg-dev)

- **libtga** (git clone git://github.com/madebr/libtga). Пример сборки через cmake:
&ensp;cd libtga
&ensp;mkdir build
&ensp;cd build
&ensp;cmake .. (вообще по умолчанию собирает как *.a библиотеку, есть опция для
&ensp;сборки как shared object, но cmake -DBUILD_SHARED не получается)
&ensp;make -j 6
&ensp;make install  
  
В папке 3d_party:

- **tinygltf** (git clone git://github.com/syoyo/tinygltf) - TinyGLTF is a header only C++11 glTF 2.0 reader.  
- **obj-io** (git clone git://github.com/thinks/obj-io) - a single-file, header-only, no-dependencies C++ implementation of the OBJ file format.  
- **Bullet physics** (git clone git://github.com/bulletphysics/bullet3) -  real-time collision detection and multi-physics simulation for VR, games, visual effects, robotics, machine learning etc. Для установки в произвольную директорию делаем так - cmake -DCMAKE_INSTALL_PREFIX:PATH=~/code/demos/3d_party/bullet3/ . && make install
  