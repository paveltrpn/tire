
#pragma once

#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include "context_bare.h"
#include "render/rendervk.h"

struct BareWindow final {
    BareWindow();

    BareWindow( const BareWindow& other ) = delete;
    BareWindow( BareWindow&& other ) = delete;
    auto operator=( const BareWindow& other ) -> BareWindow& = delete;
    auto operator=( BareWindow&& other ) -> BareWindow& = delete;

    ~BareWindow();

    auto loop() -> void;

private:
    bool run_{ true };

    GLFWwindow* window_{};
    GLFWmonitor* monitor_{};

    // window properties
    int posx_{};
    int posy_{};
    int width_{ 1024 };
    int height_{ 1024 };

    // Render related
    std::unique_ptr<tire::vk::ContextBare> context_;
    std::unique_ptr<tire::RenderVK> render_;

    uint32_t currentFrame_{ 0 };
};
