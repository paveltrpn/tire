
#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "../pipelines/pipeline.h"
#include "../command_pool.h"

namespace tire::vk {

struct RenderFromShader;

template <typename Derived>
struct RenderCommandBufferBase {
    RenderCommandBufferBase( const vk::Device *device, const CommandPool *pool )
        : device_{ device } {
        const VkCommandBufferAllocateInfo allocInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = pool->handle(),
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1 };

        if ( const auto err = vkAllocateCommandBuffers(
                 device_->handle(), &allocInfo, &commandBuffer_ );
             err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to allocate command buffers with code {}!",
                             string_VkResult( err ) ) );
        } else {
            // log::debug<DEBUG_OUTPUT_COMMAND_POOL_CPP>(
            // "vk::CommandBuffer === created!" );
        };
    }

    RenderCommandBufferBase( const RenderCommandBufferBase &other ) = delete;
    RenderCommandBufferBase( RenderCommandBufferBase &&other ) = delete;
    RenderCommandBufferBase &operator=( const RenderCommandBufferBase &other ) =
        delete;
    RenderCommandBufferBase &operator=( RenderCommandBufferBase &&other ) =
        delete;

    void reset() {
        auto self = static_cast<Derived *>( this );
        self->reset();
    }

    void prepare( VkFramebuffer framebuffer, const vk::Pipeline *pipeline ) {
        auto self = static_cast<Derived *>( this );
        self->prepare( framebuffer, pipeline );
    }

    void submit( const std::vector<VkSemaphore> &waitSemaphores,
                 const std::vector<VkSemaphore> &signalSemaphores,
                 VkFence fence ) {
        auto self = static_cast<Derived *>( this );
        self->submit( waitSemaphores, signalSemaphores, fence );
    }

protected:
    const vk::Device *device_{};
    VkCommandBuffer commandBuffer_{ VK_NULL_HANDLE };
};

// ==========================================================================

struct RenderFromShader final : RenderCommandBufferBase<RenderFromShader> {
    friend RenderCommandBufferBase;

    RenderFromShader( const vk::Device *device, const CommandPool *pool )
        : RenderCommandBufferBase<RenderFromShader>( device, pool ){};

private:
    void reset();
    void prepare( VkFramebuffer framebuffer, const vk::Pipeline *pipeline );
    void submit( const std::vector<VkSemaphore> &waitSemaphores,
                 const std::vector<VkSemaphore> &signalSemaphores,
                 VkFence fence );
};

}  // namespace tire::vk
