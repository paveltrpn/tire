# cpp_gl
Сборник простых графических программ. 

Библиотеки, необходимые для сборки:    
- SDL2 ($ sudo apt-get install libsdl2-dev)           
- FreeGLUT ($ sudo apt-get install freeglut3-dev)   
- GLFW ($ sudo apt-get install libglfw3, libglfw3-dev)
- GLEW ($ sudo apt-get install linglew2.0, libglew-dev)
- libjpeg ($ sudo apt-get install libjpeg-dev)

- libtga ($git clone git://github.com/madebr/libtga)
  сборка libtga:
  $ cd libtga
  $ mkdir build
  $ cd build
  $ cmake .. (вообще по умолчанию собирает как *.a библиотеку, есть опция для
  сборки как shared object, но cmake -DBUILD_SHARED не получается)
  $ make -j 6
  $make install 