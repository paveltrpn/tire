
# demo 005_rocket

Несложная демка под Windows. Собираем через mingw32-make.  

Зависимости:  

- MinGW-W64 (gcc 8.1.0) Скачиваем <http://mingw-w64.org/doku.php/download>

- GLFW 3.3. Скачиваем <https://www.glfw.org/download.html>. Либы копирум в %installdir%\mingw64\x86_64-w64-mingw32\lib,  
заголовки в %installdir%\mingw64\x86_64-w64-mingw32\include, копируем всю папку GLFW. Так же GLFW легко собирается cmake-ом  
из репозитория - <https://github.com/glfw/glf>. Сборка - cmake -G "MinGW Makefiles" ../

- GLEW 2.2. Скачиваем <http://glew.sourceforge.net>. Заголовки копируем в %installdir%\mingw64\x86_64-w64-mingw32\include\GL,  
либы в %installdir%\mingw64\x86_64-w64-mingw32\lib (несмотря на то, что они .lib а не .a), dll кидаем в system32 или  
в папку с exe-шником. Можно собрать cmake-ом из <https://github.com/Perlmint/glew-cmake>

- libtga. Скачиваем git clone git://github.com/madebr/libtga. Собираем через cmake, инструкция - <https://paveltrpn.github.io/draft.html>  
Перед сборкой нужно пропатчить для избежания bzero(). Заголовки и скомпилированная библиотека перемещаются как в пунктах выше.

- libjpeg. Собираем из репозитория <https://github.com/stohrendorf/libjpeg-cmake>. Собственно библиотека - liblibjpeg.a.
Заголовочные файлы - jconfig.h, jerror.h, jinclude.h, jmorecfg.h, jpeglib.h.
Можно использовать - <https://github.com/libjpeg-turbo/libjpeg-turbo>.
