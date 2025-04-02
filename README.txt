tiny_render
===========
Vulkan and OpenGL (highly likely will be dropped in future) driven graphics application.
Utilize libuv for i/o event loop. Nowdays render also appears in event loop (by timer or idle callback) but 
later there will be seperate threads for render and i/o. It stick to linux and X11 (xlib) and did't
use any window creation library such as glfw or SDL, and use some linux related math routines
such as sincos().

Build:
- - - 
Need C++23 compatible compiler and library (tested with clang++19 and clang++20).
Default cmake build script configured for clang.
Build as usual cmake project, clone and:
$ mkdir build && cd build
$ cmake ..
$ ninja -j {num threads)
Executables will appear in /{some}/{path}/tiny_render/bin

Dependencies:
- - - - - - -
OpenGL headers

Vulkan SDK:
https://vulkan.lunarg.com/sdk/home

Xlib:
$ apt install xlib-dev

libuv:
$ git clone https://github.com/libuv/libuv
$ cd libuv
$ mkdir build && cd build
$ cmake .. && cmake --install . --prefix=/{some}/{path}
or use distribution provided package

nlohmann-json:
$ git clone https://github.com/nlohmann/json

CMake build script expects some env vars:
- - - - - - - - - - - - - - - - - - - - -
libuv
$ export LIBUV_DIR=/{some}/{path}/libuv

nlohmann-json
$ export NLOHMANN_JSON_DIR=/{some}/{path}/json

vulkan sdk
$ export VULKAN_SDK=/{some}/{path}/vulkansdk_{version}/x86_64
$ export VK_LAYER_PATH=$VULKAN_SDK/share/vulkan/explicit_layer.d
$ export VK_SDK_PATH=$VULKAN_SDK
$ export CPATH=$CPATH:$VULKAN_SDK/include
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$VULKAN_SDK/lib
$ export PATH=$PATH:$VULKAN_SDK/bin


