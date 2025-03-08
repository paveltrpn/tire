
#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "device.h"
#include "render/vk/command_pool.h"
#include "surface.h"
#include "pipelines/pipeline.h"

namespace tire::vk {

struct Swapchain final {
    Swapchain( const vk::Device *device, const vk::Surface *surface,
               const vk::CommandPool *commandPool );

    Swapchain( const Swapchain &other ) = delete;
    Swapchain( Swapchain &&other ) = delete;
    Swapchain &operator=( const Swapchain &other ) = delete;
    Swapchain &operator=( Swapchain &&other ) = delete;

    ~Swapchain();

    void createSwapchain();
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
    [[nodiscard]] uint32_t swapchainImageCount() const {
        return swapchainImageCount_;
    };

private:
    const vk::Device *device_{};
    const vk::Surface *surface_{};
    const vk::CommandPool *commandPool_{};

    uint32_t swapchainImageCount_{};

    // Vulkan entity handles
    VkSwapchainKHR swapchain_{ VK_NULL_HANDLE };

    std::vector<VkImage> swapChainImages_{};
    std::vector<VkImageView> swapChainImageViews_{};
    std::vector<VkFramebuffer> framebuffers_{};
    VkFormat swapChainImageFormat_{};

    // Depth buffer
    VkImage depthImage_;
    VkDeviceMemory depthImageMemory_;
    VkImageView depthImageView_;

    // Application wide viewport render area size, use
    // it anywhere when viewport size is needed
    VkExtent2D swapChainExtent_{};
};

}  // namespace tire::vk
