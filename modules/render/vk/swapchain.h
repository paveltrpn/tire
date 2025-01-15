
#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "device.h"
#include "surface.h"
#include "pipeline.h"

namespace tire::vk {

struct Swapchain final {
    Swapchain( const vk::Device *device );

    Swapchain( const Swapchain &other ) = delete;
    Swapchain( Swapchain &&other ) = delete;
    Swapchain &operator=( const Swapchain &other ) = delete;
    Swapchain &operator=( Swapchain &&other ) = delete;

    ~Swapchain();

    void createSwapchain( const vk::Device *device,
                          const vk::Surface *surface );
    void createImageViews( const vk::Device *device );
    void createFramebuffers( const vk::Device *device,
                             const vk::Pipeline *pipeline );

    [[nodiscard]] VkSwapchainKHR swapchain() const { return swapchain_; }
    [[nodiscard]] VkFormat imageFormat() const {
        return swapChainImageFormat_;
    };

    [[nodiscard]] VkFramebuffer framebuffer( size_t id ) const {
        return framebuffers_[id];
    };

private:
    const VkDevice device_;

    VkSwapchainKHR swapchain_{ VK_NULL_HANDLE };

    std::vector<VkImage> swapChainImages_{};
    std::vector<VkImageView> swapChainImageViews_{};
    std::vector<VkFramebuffer> framebuffers_{};
    VkFormat swapChainImageFormat_{};
    VkExtent2D swapChainExtent_{};
};

}  // namespace tire::vk
