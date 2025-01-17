
#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "pipeline.h"
#include "command_pool.h"

namespace tire::vk {

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
