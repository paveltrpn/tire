## Tire editor  
  
Dependencies  

1) VulkanSceneGraph  
Add export path - vsg_DIR=/{some}/{path}/{to}/vsg  
```
git clone https://github.com/vsg-dev/VulkanSceneGraph
mkdir build && cmake ..
ninja -j {n}
cmake --install . --prefix /{some}/{path}/{to}/vsg
```
  
2) vsgQt  
Add export path - vsgqt_DIR=/{some}/{path}/{to}/vsgQt
```
git clone https://github.com/vsg-dev/vsgQt
mkdir build
cmake .. -DQT_PACKAGE_NAME=Qt6  -DCMAKE_PREFIX_PATH=/{path}/{to}/Qt6
ninja -j {n}
cmake --install . --prefix /{some}/{path}/{to}/vsgQt
```
  
2) vsgXchange  
Add export path - vsgXchange_DIR=/{some}/{path}/{to}/vsgXchange
```
git clone https://github.com/vsg-dev/vsgXchange
mkdir build && cmake ..
ninja -j {n}
cmake --install . --prefix /{some}/{path}/{to}/vsgXchange
```



