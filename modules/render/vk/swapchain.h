
#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "device.h"
#include "surface.h"
#include "pipeline.h"

namespace tire::vk {

struct Swapchain final {
    Swapchain( const vk::Device *device, const vk::Surface *surface );

    Swapchain( const Swapchain &other ) = delete;
    Swapchain( Swapchain &&other ) = delete;
    Swapchain &operator=( const Swapchain &other ) = delete;
    Swapchain &operator=( Swapchain &&other ) = delete;

    ~Swapchain();

    void createSwapchain( VkExtent2D extent );
    void createImageViews();
    void createFramebuffers( const vk::Pipeline *pipeline );

    [[nodiscard]] VkSwapchainKHR handle() const { return swapchain_; }
    [[nodiscard]] VkFormat imageFormat() const {
        return swapChainImageFormat_;
    };

    [[nodiscard]] VkFramebuffer framebuffer( size_t id ) const {
        return framebuffers_[id];
    };

    [[nodiscard]] VkExtent2D extent() const { return swapChainExtent_; }

private:
    const vk::Device *device_{};
    const vk::Surface *surface_{};

    // Vulkan etity handles
    VkSwapchainKHR swapchain_{ VK_NULL_HANDLE };

    std::vector<VkImage> swapChainImages_{};
    std::vector<VkImageView> swapChainImageViews_{};
    std::vector<VkFramebuffer> framebuffers_{};
    VkFormat swapChainImageFormat_{};

    // Application wide viewport render area size, use
    // it anywhere when viewport size is needed
    VkExtent2D swapChainExtent_{};
};

}  // namespace tire::vk
