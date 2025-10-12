
#ifndef __bare_window_h__
#define __bare_window_h__

#include <iostream>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

#include <vulkan/vulkan.h>

#include "render/context.h"
#include "render/rendervk.h"
#include "scene/scene.h"

struct BareWindow final {
    BareWindow();

    ~BareWindow();

    auto loop() -> void;

private:
    GLFWwindow* window_{};
    GLFWmonitor* monitor_{};

    // window properties
    int posx_{};
    int posy_{};
    int width_{ 1024 };
    int height_{ 1024 };

    // Render related
    std::unique_ptr<tire::vk::Context> context_;
    std::unique_ptr<tire::RenderVK> render_;
};

#endif
