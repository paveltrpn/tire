
#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <type_traits>

#include "../pipelines/pipeline.h"
#include "../command_pool.h"

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_COMMANDS_H{ true };
#include "image/color.h"

namespace tire::vk {

struct RenderFromShader;
struct RenderFromBuffer;

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

    // Overload for "RenderFromShader".
    // Args waits:
    // @param count uint32_t - number of vertcies count to render
    template <typename... Args>
    requires( std::is_same_v<
              derived_type,
              RenderFromShader> ) void setProperties( VkFramebuffer framebuffer,
                                                      const vk::Pipeline
                                                          *pipeline,
                                                      Args &&...args ) {
        pipeline_ = pipeline;
        framebuffer_ = framebuffer;
        impl()->setLocalState( args... );
    }

    void reset() { vkResetCommandBuffer( commandBuffer_, 0 ); }

    void bind() {
        preamble();
        impl()->bind_impl();
    }

    void submit( VkSemaphore waitSemaphores, VkSemaphore signalSemaphores,
                 VkFence fence ) {
        std::array<VkPipelineStageFlags, 1> waitStages = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        std::array<VkSemaphore, 1> waitsems{ waitSemaphores };
        std::array<VkSemaphore, 1> sgnlsems{ signalSemaphores };

        const VkSubmitInfo submitInfo{ .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                                       .waitSemaphoreCount = 1,
                                       .pWaitSemaphores = waitsems.data(),
                                       .pWaitDstStageMask = waitStages.data(),
                                       .commandBufferCount = 1,
                                       .pCommandBuffers = &commandBuffer_,
                                       .signalSemaphoreCount = 1,
                                       .pSignalSemaphores = sgnlsems.data() };

        // NOTE: omit return code check
        vkQueueSubmit( device_->graphicsQueue(), 1, &submitInfo, fence );
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

        const auto cc = Colorf{ "darkblue" };
        clearColor_.color = { cc.r(), cc.g(), cc.b(), cc.a() },
        clearColor_.depthStencil = { .depth = 0.0f, .stencil = 0 };

        width_ = device_->extent().width;
        height_ = device_->extent().height;
    }

private:
    derived_type *impl() { return static_cast<derived_type *>( this ); }

    void preamble() {
        const VkCommandBufferBeginInfo beginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .flags = 0,
            .pInheritanceInfo = nullptr };
        // NOTE: omit return code check
        vkBeginCommandBuffer( commandBuffer_, &beginInfo );

        const VkRenderPassBeginInfo renderPassInfo{
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext = nullptr,
            .renderPass = pipeline_->renderpass(),
            .framebuffer = framebuffer_,
            .renderArea = { .offset = { .x = 0, .y = 0 },
                            .extent = { device_->extent() } },
            .clearValueCount = 1,
            .pClearValues = &clearColor_ };

        vkCmdBeginRenderPass( commandBuffer_, &renderPassInfo,
                              VK_SUBPASS_CONTENTS_INLINE );

        vkCmdBindPipeline( commandBuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS,
                           pipeline_->pipeline() );

        // Dynamic viewport. No performance penalty.
        // Take out work from pipeline creation.
        const VkViewport viewport{ .x = 0.0f,
                                   .y = 0.0f,
                                   .width = static_cast<float>( width_ ),
                                   .height = static_cast<float>( height_ ),
                                   .minDepth = 0.0f,
                                   .maxDepth = 1.0f };
        vkCmdSetViewport( commandBuffer_, 0, 1, &viewport );

        const VkRect2D scissor{ { .x = 0, .y = 0 },
                                { .width = width_, .height = height_ } };
        vkCmdSetScissor( commandBuffer_, 0, 1, &scissor );
    }

protected:
    // Propertis set on "command" creation and stay same trough lifetime
    const vk::Device *device_{};
    const vk::CommandPool *pool_{};
    VkCommandBuffer commandBuffer_{ VK_NULL_HANDLE };
    VkClearValue clearColor_{};
    uint32_t width_{};
    uint32_t height_{};

    // Properties that can varies on each submit
    const vk::Pipeline *pipeline_{};
    VkFramebuffer framebuffer_{};
};

// ==========================================================================

struct RenderFromShader final : DrawCommand<RenderFromShader> {
    friend DrawCommand;
    using base_type = DrawCommand<RenderFromShader>;

    RenderFromShader( const vk::Device *device, const CommandPool *pool )
        : base_type( device, pool ) {};

private:
    void bind_impl();
    void setLocalState( uint32_t count ) { verteciesCount_ = count; };

private:
    // Properties that can varies on each submit
    uint32_t verteciesCount_{};
};

}  // namespace tire::vk
