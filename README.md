# Tire Editor

A Vulkan-based 3D scene editor with Qt-based UI, featuring real-time scenegraph manipulation and procedural primitive generation.

## About

Tire Editor is a lightweight 3D scene editor built with:

- **Vulkan** via VulkanSceneGraph (VSG) for modern GPU rendering
- **Qt 6.10** for the user interface
- **C++26** with experimental reflection support (`-freflection`)

The editor provides a split-pane UI with a 3D viewport, object manipulation controls, and dynamic primitive generation (boxes, spheres, cylinders) with configurable properties.

## Dependencies Installation

### Vulkan SDK

```bash
# Download and install from https://vulkan.lunarg.com/sdk/home
# Or use package manager:
sudo apt install vulkan-sdk
```

### glslang

```bash
git clone https://github.com/KhronosGroup/glslang
cd glslang
./update_glslang_sources.py
mkdir build && cd build
cmake ..
cmake --install . --prefix /path/to/glslang
export glslang_DIR=/path/to/glslang
```

### VulkanSceneGraph (VSG)

```bash
git clone https://github.com/vsg-dev/VulkanSceneGraph
cd VulkanSceneGraph
mkdir build && cd build
cmake ..
ninja -j {n}
cmake --install . --prefix /path/to/vsg
export vsg_DIR=/path/to/vsg
```

### Qt 6.10+

```bash
# Download from https://www.qt.io/download
# Or use package manager:
sudo apt install qt6-base-dev qt6-declarative-dev
```

## Build

```bash
# Set required environment variables
export VULKAN_SDK=/path/to/vulkan/sdk
export glslang_DIR=/path/to/glslang
export vsg_DIR=/path/to/vsg
export LOCAL_LATEST_QT=/path/to/qt
export GCC_LATEST=/path/to/gcc

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build .
```

