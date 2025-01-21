
#pragma once

#include <vulkan/vulkan.h>
#include <type_traits>

#include "../pipelines/pipeline.h"
#include "../command_pool.h"

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_COMMANDS_H{ true };

namespace tire::vk {

struct RenderFromShader;
struct DummyCommand;

/* Command depends on:
 * 1) Device
 * 2) Pipline
 * 3) Swapchain->Framebuffer
 * 4) Synchronization
 */

template <typename Derived>
struct DrawCommand {
    using value_type = Derived;

    DrawCommand( const vk::Device *device, const CommandPool *pool )
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
            log::debug<DEBUG_OUTPUT_COMMANDS_H>(
                "vk::DrawCommand === created!" );
        };
    }

    DrawCommand( const DrawCommand &other ) = delete;
    DrawCommand( DrawCommand &&other ) = delete;
    DrawCommand &operator=( const DrawCommand &other ) = delete;
    DrawCommand &operator=( DrawCommand &&other ) = delete;

    void reset() {
        auto self = static_cast<value_type *>( this );
        self->reset();
    }

    void prepare( VkFramebuffer framebuffer, const vk::Pipeline *pipeline ) {
        auto self = static_cast<value_type *>( this );
        self->prepare( framebuffer, pipeline );
    }

    void submit( VkSemaphore waitSemaphores, VkSemaphore signalSemaphores,
                 VkFence fence ) {
        auto self = static_cast<value_type *>( this );
        self->submit( waitSemaphores, signalSemaphores, fence );
    }

    template <typename... Args>
    requires( std::is_same_v<value_type, DummyCommand> ) void update(
        Args &&...args ) {
        auto self = static_cast<value_type *>( this );
        self->update( args... );
    }

    template <typename... Args>
    requires( std::is_same_v<value_type, RenderFromShader> ) void update(
        Args &&...args ) {
        auto self = static_cast<value_type *>( this );
        self->setVerteciesCount( args... );
    }

protected:
    const vk::Device *device_{};
    VkCommandBuffer commandBuffer_{ VK_NULL_HANDLE };
};

// ==========================================================================

struct DummyCommand final : DrawCommand<DummyCommand> {
    friend DrawCommand;

private:
    DummyCommand( const vk::Device *device, const CommandPool *pool )
        : DrawCommand<DummyCommand>( device, pool ){};

    void reset(){};
    void prepare( VkFramebuffer framebuffer, const vk::Pipeline *pipeline ){};
    void submit( VkSemaphore waitSemaphores, VkSemaphore signalSemaphores,
                 VkFence fence ){};
    void update() { log::notice( "called from DummyCommand " ); }
};

// ==========================================================================

struct RenderFromShader final : DrawCommand<RenderFromShader> {
    friend DrawCommand;

private:
    RenderFromShader( const vk::Device *device, const CommandPool *pool )
        : DrawCommand<RenderFromShader>( device, pool ){};

    void reset();
    void prepare( VkFramebuffer framebuffer, const vk::Pipeline *pipeline );
    void submit( VkSemaphore waitSemaphores, VkSemaphore signalSemaphores,
                 VkFence fence );
    void setVerteciesCount( uint32_t count ) { verteciesCount_ = count; };

private:
    uint32_t verteciesCount_{};
};

}  // namespace tire::vk
