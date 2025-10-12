
#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <vulkan/vulkan_core.h>

namespace tire::vk {

struct Pipeline;

struct Context {
    Context() = default;
    virtual ~Context() = default;

    Context( const Context& other ) = delete;
    Context( Context&& other ) = delete;
    auto operator=( const Context& other ) -> Context& = delete;
    auto operator=( Context&& other ) -> Context& = delete;

    [[nodiscard]] auto instance() const -> VkInstance { return instance_; };
    [[nodiscard]] auto surface() const -> VkSurfaceKHR { return surface_; };
    [[nodiscard]] auto device() const -> VkDevice { return device_; };
    [[nodiscard]] auto commandPool() const -> VkCommandPool {
        return commandPool_;
    };
    [[nodiscard]] auto swapchain() const -> VkSwapchainKHR {
        return swapchain_;
    };

    [[nodiscard]]
    const VkSurfaceFormatKHR& surfaceFormat() const {
        return surfaceFormat_;
    };

    [[nodiscard]]
    const VkQueue& graphicsQueue() const {
        return graphicsQueue_;
    }

    [[nodiscard]] auto graphicsFamily() const -> uint32_t {
        return graphicsFamilyQueueId_;
    };

    [[nodiscard]]
    const VkExtent2D& currentExtent() const {
        return currentExtent_;
    };

    [[nodiscard]]
    virtual uint32_t memoryRequirements(
        uint32_t typeFilter, VkMemoryPropertyFlags properties ) const = 0;

    [[nodiscard]]
    virtual VkFormat findSupportedFormat(
        const std::vector<VkFormat>& candidates, VkImageTiling tiling,
        VkFormatFeatureFlags features ) const = 0;

    [[nodiscard]]
    virtual auto getDrawCommandBuffer() const -> VkCommandBuffer = 0;

protected:
    // Instance
    VkInstance instance_{ VK_NULL_HANDLE };
    VkSurfaceKHR surface_{ VK_NULL_HANDLE };
    VkDevice device_{ VK_NULL_HANDLE };
    VkCommandPool commandPool_{ VK_NULL_HANDLE };
    VkSwapchainKHR swapchain_{ VK_NULL_HANDLE };

    uint32_t graphicsFamilyQueueId_{ UINT32_MAX };

    VkExtent2D currentExtent_{};

    VkQueue graphicsQueue_{ VK_NULL_HANDLE };

    VkSurfaceFormatKHR surfaceFormat_{};
};

}  // namespace tire::vk