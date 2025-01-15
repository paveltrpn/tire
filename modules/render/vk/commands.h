
#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "pipeline.h"

namespace tire::vk {

struct CommandPool final {
    CommandPool( const VkDevice device )
        : device_{ device } {};

    CommandPool( const CommandPool &other ) = delete;
    CommandPool( CommandPool &&other ) = delete;
    CommandPool &operator=( const CommandPool &other ) = delete;
    CommandPool &operator=( CommandPool &&other ) = delete;

    ~CommandPool() { vkDestroyCommandPool( device_, commandPool_, nullptr ); }

    void init( uint32_t queueFamilyIndex );
    [[nodiscard]] VkCommandPool get() const { return commandPool_; }

private:
    const VkDevice device_{ VK_NULL_HANDLE };
    VkCommandPool commandPool_{ VK_NULL_HANDLE };
};

// =======================================================================

struct CommandBuffer final {
    CommandBuffer( const VkDevice &device, const CommandPool *pool );

    CommandBuffer( const CommandBuffer &other ) = delete;
    CommandBuffer( CommandBuffer &&other ) = delete;
    CommandBuffer &operator=( const CommandBuffer &other ) = delete;
    CommandBuffer &operator=( CommandBuffer &&other ) = delete;

    void reset();
    void beginRenderPassCommand( VkFramebuffer framebuffer,
                                 const vk::Pipeline *pipeline );
    void submit( const std::vector<VkSemaphore> &waitSemaphores,
                 const std::vector<VkSemaphore> &signalSemaphores,
                 VkFence fence, VkQueue queue );

private:
    const VkDevice device_{ VK_NULL_HANDLE };
    VkCommandBuffer commandBuffer_{ VK_NULL_HANDLE };
};

}  // namespace tire::vk
