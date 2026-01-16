
#pragma once

#include <memory>

#include <GLFW/glfw3.h>

import context;

namespace tire {
struct RenderVK;
}

struct BareWindow final {
    BareWindow();

    BareWindow( const BareWindow &other ) = delete;
    BareWindow( BareWindow &&other ) = delete;
    auto operator=( const BareWindow &other ) -> BareWindow & = delete;
    auto operator=( BareWindow &&other ) -> BareWindow & = delete;

    ~BareWindow();

    auto loop() -> void;

private:
    GLFWwindow *window_{};
    GLFWmonitor *monitor_{};

    // window properties
    int posx_{};
    int posy_{};
    int width_{ 1024 };
    int height_{ 1024 };

    // Render related
    std::unique_ptr<tire::RenderVK> render_;
    std::unique_ptr<tire::Context> context_;
};
