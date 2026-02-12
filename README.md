## Tire editor  
  
Dependencies  

1\. Vulkan SDK iteself  
```
install vulkan sdk
```
  
2\. glslang libraries  
Add export path - glslang_DIR=/{some}/{path}/{to}/glslang  
```
git clone https://github.com/KhronosGroup/glslang
./update_glslang_sources.py
mkdir build && cmake ..
cmake --install . --prefix /{some}/{path}/{to}/glslang

```
  
3\. VulkanSceneGraph  
Add export path - vsg_DIR=/{some}/{path}/{to}/vsg  
```
git clone https://github.com/vsg-dev/VulkanSceneGraph
mkdir build && cmake ..
ninja -j {n}
cmake --install . --prefix /{some}/{path}/{to}/vsg
```
  



