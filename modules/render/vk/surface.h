
#pragma once

#include <X11/Xlib.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "instance.h"

namespace tire::vk {

struct Surface final {
    Surface( Display *display, Window window, const vk::Instance *instance );

    Surface( const Surface &other ) = delete;
    Surface( Surface &&other ) = delete;
    Surface &operator=( const Surface &other ) = delete;
    Surface &operator=( Surface &&other ) = delete;

    ~Surface();

    [[nodiscard]] VkSurfaceKHR handle() const;

private:
    const vk::Instance *instance_{};
    VkSurfaceKHR surface_{ VK_NULL_HANDLE };
};

}  // namespace tire::vk
