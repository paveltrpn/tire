
#include <vulkan/vk_enum_string_helper.h>

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_COMMAND_POOL_CPP{ true };

#include "image/color.h"

#include "commands.h"

namespace tire::vk {

void CommandPool::init( uint32_t queueFamilyIndex ) {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndex;

    const auto err = vkCreateCommandPool( device_->handle(), &poolInfo, nullptr,
                                          &commandPool_ );
    if ( err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create command pool woth code {}!",
                         string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_COMMAND_POOL_CPP>( "command pool created!" );
    }
}

// =======================================================================

CommandBuffer::CommandBuffer( const vk::Device *device,
                              const CommandPool *pool )
    : device_{ device } {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = pool->get();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    const auto err = vkAllocateCommandBuffers( device_->handle(), &allocInfo,
                                               &commandBuffer_ );
    if ( err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to allocate command buffers with code {}!",
                         string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_COMMAND_POOL_CPP>( "command buffer created!" );
    };
}

void CommandBuffer::reset() {
    vkResetCommandBuffer( commandBuffer_, 0 );
}

void CommandBuffer::beginRenderPassCommand( VkFramebuffer framebuffer,
                                            const vk::Pipeline *pipeline ) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;                   // Optional
    beginInfo.pInheritanceInfo = nullptr;  // Optional

    {
        const auto err = vkBeginCommandBuffer( commandBuffer_, &beginInfo );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error( std::format(
                "failed to begin recording command buffer with code {}!",
                string_VkResult( err ) ) );
        }
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = pipeline->renderpass();
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent =
        VkExtent2D{ static_cast<uint32_t>( pipeline->viewport_.width ),
                    static_cast<uint32_t>( pipeline->viewport_.height ) };

    const auto clearColor = Colorf{ "darkblue" };
    VkClearValue clearColorValue = { { { clearColor.r(), clearColor.g(),
                                         clearColor.b(), clearColor.a() } } };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColorValue;

    vkCmdBeginRenderPass( commandBuffer_, &renderPassInfo,
                          VK_SUBPASS_CONTENTS_INLINE );

    vkCmdBindPipeline( commandBuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS,
                       pipeline->pipeline() );

    // vertexCount: Even though we don’t have a vertex buffer, we technically
    // still have 3 vertices to draw.

    // instanceCount: Used for instanced rendering, use 1 if you’re not doing
    // that.

    // firstVertex: Used as an offset into the vertex buffer, defines the lowest
    // value of gl_VertexIndex.

    // firstInstance: Used as an offset for instanced rendering, defines the
    // lowest value of gl_InstanceIndex.
    vkCmdDraw( commandBuffer_, 3, 1, 0, 0 );

    vkCmdEndRenderPass( commandBuffer_ );

    {
        const auto err = vkEndCommandBuffer( commandBuffer_ );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to record command buffer with code {}!",
                             string_VkResult( err ) ) );
        }
    }
}

void CommandBuffer::submit( const std::vector<VkSemaphore> &waitSemaphores,
                            const std::vector<VkSemaphore> &signalSemaphores,
                            VkFence fence, VkQueue queue ) {
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores.data();
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer_;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores.data();

    const auto err = vkQueueSubmit( queue, 1, &submitInfo, fence );
    if ( err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to submit draw command buffer with code {}!",
                         string_VkResult( err ) ) );
    }
}

}  // namespace tire::vk
