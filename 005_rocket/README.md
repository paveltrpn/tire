
# demo 005_rocket

Несложная демка под Windows. Собираем через mingw32-make.  

Зависимости:  

- MinGW-W64 (gcc 8.1.0) Скачиваем <http://mingw-w64.org/doku.php/download>

- GLFW 3.3. Скачиваем <https://www.glfw.org/download.html>. Либы копирум в %installdir%\mingw64\x86_64-w64-mingw32\lib,  
заголовки в %installdir%\mingw64\x86_64-w64-mingw32\include, копируем всю папку GLEW

- GLEW 2.2. Скачиваем <http://glew.sourceforge.net>. Заголовки копируем в %installdir%\mingw64\x86_64-w64-mingw32\include\GL,  
либы в %installdir%\mingw64\x86_64-w64-mingw32\lib (несмотря на то, что они .lib а не .a), dll кидаем в system32 или  
в папку с exe-шником.
