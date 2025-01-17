
#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "pipeline.h"

namespace tire::vk {

struct CommandPool final {
    CommandPool( const vk::Device *device );

    CommandPool( const CommandPool &other ) = delete;
    CommandPool( CommandPool &&other ) = delete;
    CommandPool &operator=( const CommandPool &other ) = delete;
    CommandPool &operator=( CommandPool &&other ) = delete;

    ~CommandPool() {
        vkDestroyCommandPool( device_->handle(), commandPool_, nullptr );
    }

    [[nodiscard]] VkCommandPool handle() const { return commandPool_; }

private:
    const vk::Device *device_{};

    // Vulkan etity handles
    VkCommandPool commandPool_{ VK_NULL_HANDLE };

    // Vulkan related structures
    VkCommandPoolCreateInfo poolInfo_{};
};

// =======================================================================

struct CommandBuffer final {
    CommandBuffer( const vk::Device *device, const CommandPool *pool );

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
    const vk::Device *device_{};
    VkCommandBuffer commandBuffer_{ VK_NULL_HANDLE };
};

}  // namespace tire::vk
