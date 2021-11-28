# Demos

Сборник простых графических программ.  

Библиотеки, необходимые для сборки, должны лежать в ./extern:  

- **fmt::format V8.0.1** - (git clone git://github.com/fmtlib/fmt) - A modern formatting library 
- **Bullet physics V3.1** (git clone git://github.com/bulletphysics/bullet3) - real-time collision detection and multi-physics simulation for VR, games, visual effects,
- **Dear ImGui V1.84.2** (git clone git://github.com/ocornut/imgui) Bloat-free Graphical User interface for C++ with minimal dependencies  
- **SDL2** (git clone https://github.com/libsdl-org/SDL; sudo apt-get install libsdl2-dev)  
- **GLFW** (git clone git://github.com/glfw/glfw)
- **GLEW** можно скачать с git clone git://github.com/nigels-com/glew или git://github.com/Perlmint/glew-cmake, но, как сказано в issues репозитория git://github.com/nigels-com/glew - "On windows the general advice is to build from the release archive, rather than git repository.". Собственно release archive  лежит в "http://glew.sourceforge.net/", откуда качаем архив и просто собираем make-ом. Можно линковать статически добавив glew.c в проект и #define GLEW_STATIC к каждому включению glew.h
- **libjpeg** (git clone git://github.com/stohrendorf/libjpeg-cmake)
- **libtga** (git clone git://github.com/madebr/libtga) В ней нужно заменить 4-ре вхождения функции bzero() на memset(ptr, value, size) в *.c файлах  
- **GLM** (gin clone https://github.com/g-truc/glm) OpenGL Mathematics (GLM) is a header only C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications.
  
017_Water - Демо с моделированием воды, взято с gamedev.ru  
TODO - ИСПРАВИТЬ! Крашится после нескольких нажатий на кнопку сброса параметров плоскости или длительной работы приложения, есть версия что причина - неуклюжая работа  
с std::vector<> (контейнеры с точками и нормалями) или указателями на них. UPD - убрал ссылки на вектора.  
