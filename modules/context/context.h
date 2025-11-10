
#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace tire::vk {

struct Context {
    Context() = default;
    virtual ~Context() = default;

    Context( const Context& other ) = delete;
    Context( Context&& other ) = delete;
    auto operator=( const Context& other ) -> Context& = delete;
    auto operator=( Context&& other ) -> Context& = delete;

    virtual auto init() -> void = 0;

    [[nodiscard]] auto instance() const -> VkInstance { return instance_; };
    [[nodiscard]] auto surface() const -> VkSurfaceKHR { return surface_; };
    [[nodiscard]] auto device() const -> VkDevice { return device_; };
    [[nodiscard]] auto swapchain() const -> VkSwapchainKHR {
        return swapchain_;
    };

    [[nodiscard]]
    auto surfaceFormat() const -> const VkSurfaceFormatKHR& {
        return surfaceFormat_;
    };

    [[nodiscard]]
    auto graphicsQueue() const -> const VkQueue& {
        return graphicsQueue_;
    }

    [[nodiscard]] auto graphicsFamily() const -> uint32_t {
        return graphicsFamilyQueueId_;
    };

    [[nodiscard]]
    virtual auto currentExtent() const -> const VkExtent2D& {
        return currentExtent_;
    };

    auto setViewportSize( uint32_t width, uint32_t height ) -> void {
        width_ = width;
        height_ = height;
    }

    [[nodiscard]]
    auto viewportSize() -> std::tuple<uint32_t, uint32_t> const {
        return { width_, height_ };
    }

    [[nodiscard]]
    auto memoryRequirements( uint32_t typeFilter,
                             VkMemoryPropertyFlags properties ) const
        -> uint32_t;

    [[nodiscard]]
    auto findSupportedFormat( const std::vector<VkFormat>& candidates,
                              VkImageTiling tiling,
                              VkFormatFeatureFlags features ) const -> VkFormat;

    [[nodiscard]] auto renderPass() const -> VkRenderPass {
        return renderPass_;
    };

protected:
    VkInstance instance_{ VK_NULL_HANDLE };
    VkSurfaceKHR surface_{ VK_NULL_HANDLE };
    VkPhysicalDevice physDevice_{};
    VkDevice device_{ VK_NULL_HANDLE };
    VkSwapchainKHR swapchain_{ VK_NULL_HANDLE };

    VkQueue graphicsQueue_{ VK_NULL_HANDLE };
    VkSurfaceFormatKHR surfaceFormat_{};
    VkRenderPass renderPass_{ VK_NULL_HANDLE };

    uint32_t graphicsFamilyQueueId_{ UINT32_MAX };

    VkExtent2D currentExtent_{};

    uint32_t width_{};
    uint32_t height_{};
};

}  // namespace tire::vk