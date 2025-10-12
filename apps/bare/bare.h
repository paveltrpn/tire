
#ifndef __bare_window_h__
#define __bare_window_h__

#include <iostream>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

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
    int width_{ 640 };
    int height_{ 480 };
};

#endif
