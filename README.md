# Demos

Сборник простых графических программ.  

Библиотеки, необходимые для сборки:  

- **SDL2** (sudo apt-get install libsdl2-dev)  
- **FreeGLUT** (sudo apt-get install freeglut3-dev)  
- **GLFW** (sudo apt-get install libglfw3 libglfw3-dev)
- **GLEW** (sudo apt-get install libglew2.0 libglew-dev)
- **libjpeg** (sudo apt-get install libjpeg-dev)

- **libtga** (git clone git://github.com/madebr/libtga). Пример сборки через cmake:
  cd libtga
  mkdir build
  cd build
  cmake .. (вообще по умолчанию собирает как *.a библиотеку, есть опция для
  сборки как shared object, но cmake -DBUILD_SHARED не получается)
  make -j 6
  make install  
  
В папке 3d_party:

- **tinygltf** (git clone git://github.com/syoyo/tinygltf) - TinyGLTF is a header only C++11 glTF 2.0 reader.  
- **obj-io** (git clone https://github.com/thinks/obj-io) - a single-file, header-only, no-dependencies C++ implementation of the OBJ file format.  
- **Bullet physics** (git clone https://github.com/bulletphysics/bullet3) -  real-time collision detection and multi-physics simulation for VR, games, visual effects, robotics, machine learning etc.  
