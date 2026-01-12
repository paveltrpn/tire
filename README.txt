tiny_render
===========
Vulkan driven graphics application.

Build:
- - -
Need C++26 compatible compiler and library (tested with latest clang).
Build as usual cmake project, clone and:
$ mkdir build && cd build
$ cmake ..
$ ninja -j {num threads)

Dependencies:
- - - - - - -
Vulkan SDK:
https://vulkan.lunarg.com/sdk/home

VulkanMemoryAllocator
$ git clone https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator

GLFW:
$ apt install libglfw3-dev

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

VulkanMemoryAllocator
$export VMA_DIR=/{some}/{path}/{to}/VulkanMemoryAllocator

vulkan sdk
$ export VULKAN_SDK=/{some}/{path}/vulkansdk_{version}/x86_64
$ export VK_LAYER_PATH=$VULKAN_SDK/share/vulkan/explicit_layer.d
$ export VK_SDK_PATH=$VULKAN_SDK
$ export CPATH=$CPATH:$VULKAN_SDK/include
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$VULKAN_SDK/lib
$ export PATH=$PATH:$VULKAN_SDK/bin
