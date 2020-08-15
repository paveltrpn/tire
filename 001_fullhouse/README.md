# demo 001_fullhouse
Программа - каркас OpenGL приложения. Использует OpenGL v2.1, не выше.      
Включает в себя инициализацию окна с OpenGL контекстом (через GLFW и GLEW),     
таймер, загрузку изображений в форматах jpeg и tga, вывод текста в окно   
OpenGL через шрифт в текстурном атласе, атлас в формате TGA с началом в левом верхнем углу.   
    
Библиотеки, необходимые для сборки:    
- GLFW ($ sudo apt-get install libglfw3 libglfw3-dev)   
- GLEW ($ sudo apt-get install libglew2.0 libglew-dev)    
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