
# demo 006_rocket_nix

Несложная демка под Linux.  

Зависимости:  

- GLFW ($ sudo apt-get install libglfw3-dev)

- GLEW ($ sudo apt-get install libglew-dev)

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