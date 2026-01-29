## Tire editor  
  
Dependencies  

1) Vulkan SDK iteself  
```
install vulkan sdk
```
  
2) glslang clibraries  
Add export path - glslang_DIR=/{some}/{path}/{to}/glslang  
```
git clone https://github.com/KhronosGroup/glslang
./update_glslang_sources.py
mkdir build && cmake ..
cmake --install . --prefix /{some}/{path}/{to}/glslang

```
  
3) VulkanSceneGraph  
Add export path - vsg_DIR=/{some}/{path}/{to}/vsg  
```
git clone https://github.com/vsg-dev/VulkanSceneGraph
mkdir build && cmake ..
ninja -j {n}
cmake --install . --prefix /{some}/{path}/{to}/vsg
```
  
4) vsgQt  
Add export path - vsgqt_DIR=/{some}/{path}/{to}/vsgQt
```
git clone https://github.com/vsg-dev/vsgQt
mkdir build
cmake .. -DQT_PACKAGE_NAME=Qt6  -DCMAKE_PREFIX_PATH=/{path}/{to}/Qt6
ninja -j {n}
cmake --install . --prefix /{some}/{path}/{to}/vsgQt
```
  
5) vsgXchange  
Add export path - vsgXchange_DIR=/{some}/{path}/{to}/vsgXchange
```
git clone https://github.com/vsg-dev/vsgXchange
mkdir build && cmake ..
ninja -j {n}
cmake --install . --prefix /{some}/{path}/{to}/vsgXchange
```



