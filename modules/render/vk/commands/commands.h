
#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
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
    using derived_type = Derived;

    DrawCommand( const DrawCommand &other ) = delete;
    DrawCommand( DrawCommand &&other ) = delete;
    DrawCommand &operator=( const DrawCommand &other ) = delete;
    DrawCommand &operator=( DrawCommand &&other ) = delete;

    ~DrawCommand() { clean(); }

    void clean() {
        std::array<VkCommandBuffer, 1> toBeFree{ commandBuffer_ };
        vkFreeCommandBuffers( device_->handle(), pool_->handle(), 1,
                              toBeFree.data() );
    }

    void reset() { impl()->reset_impl(); }

    void prepare( VkFramebuffer framebuffer, const vk::Pipeline *pipeline ) {
        impl()->prepare_impl( framebuffer, pipeline );
    }

    void submit( VkSemaphore waitSemaphores, VkSemaphore signalSemaphores,
                 VkFence fence ) {
        impl()->submit_impl( waitSemaphores, signalSemaphores, fence );
    }

    template <typename... Args>
    requires( std::is_same_v<derived_type, DummyCommand> ) void update(
        Args &&...args ) {
        impl()->update_impl( args... );
    }

    template <typename... Args>
    requires( std::is_same_v<derived_type, RenderFromShader> ) void update(
        Args &&...args ) {
        impl()->setVerteciesCount( args... );
    }

protected:
    DrawCommand( const vk::Device *device, const CommandPool *pool )
        : device_{ device }
        , pool_{ pool } {
        const VkCommandBufferAllocateInfo allocInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = pool_->handle(),
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

private:
    derived_type *impl() { return static_cast<derived_type *>( this ); }

protected:
    const vk::Device *device_{};
    const vk::CommandPool *pool_{};
    VkCommandBuffer commandBuffer_{ VK_NULL_HANDLE };
};

// ==========================================================================

struct DummyCommand final : DrawCommand<DummyCommand> {
    friend DrawCommand;
    using base_type = DrawCommand<DummyCommand>;

    DummyCommand( const vk::Device *device, const CommandPool *pool )
        : base_type( device, pool ){};

private:
    void reset_impl(){};
    void prepare_impl( VkFramebuffer framebuffer,
                       const vk::Pipeline *pipeline ){};
    void submit_impl( VkSemaphore waitSemaphores, VkSemaphore signalSemaphores,
                      VkFence fence ){};
    void update_impl() { log::notice( "called from DummyCommand " ); }
};

// ==========================================================================

struct RenderFromShader final : DrawCommand<RenderFromShader> {
    friend DrawCommand;
    using base_type = DrawCommand<RenderFromShader>;

    RenderFromShader( const vk::Device *device, const CommandPool *pool )
        : base_type( device, pool ){};

private:
    void reset_impl();
    void prepare_impl( VkFramebuffer framebuffer,
                       const vk::Pipeline *pipeline );
    void submit_impl( VkSemaphore waitSemaphores, VkSemaphore signalSemaphores,
                      VkFence fence );
    void setVerteciesCount( uint32_t count ) { verteciesCount_ = count; };

private:
    uint32_t verteciesCount_{};
};

}  // namespace tire::vk
