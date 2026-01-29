## Tire editor.  
  
Dependencies  

1) VulkanSceneGraph  
Add export path - vsg_DIR=/mnt/main/sources_build/vsg  
```
git clone https://github.com/vsg-dev/VulkanSceneGraph
mkdir build && cmake ..
ninja -j {n}
cmake --install . --prefix /{spme}/{path}/{to}/vsg
```
  
2) vsgQt  
Add export path - vsgqt_DIR=/mnt/main/sources_build/vsgQt
```
git clone https://github.com/vsg-dev/vsgQt
mkdir build
cmake .. -DQT_PACKAGE_NAME=Qt6  -DCMAKE_PREFIX_PATH=/{path}/{to}/Qt6
ninja -j {n}
cmake --install . --prefix /{spme}/{path}/{to}/vsgQt
```



