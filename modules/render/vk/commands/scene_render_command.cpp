
#include "scene_render_command.h"

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_SCENE_RENDER_COMMAND_CPP{ true };

namespace tire::vk {

SceneRenderCommand::SceneRenderCommand( const vk::Device *device,
                                        const CommandPool *pool )
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
        log::debug<DEBUG_OUTPUT_SCENE_RENDER_COMMAND_CPP>(
            "vk::SceneRenderCommand === created!" );
    };

    const auto cc = Colorf{ "darkblue" };
    clearColor_.color = { cc.r(), cc.g(), cc.b(), cc.a() },
    clearColor_.depthStencil = { .depth = 0.0f, .stencil = 0 };

    width_ = device_->extent().width;
    height_ = device_->extent().height;
}

void SceneRenderCommand::clean() {
    std::array<VkCommandBuffer, 1> toBeFree{ commandBuffer_ };
    vkFreeCommandBuffers( device_->handle(), pool_->handle(), 1,
                          toBeFree.data() );
}

void SceneRenderCommand::reset() {
    vkResetCommandBuffer( commandBuffer_, 0 );
}

void SceneRenderCommand::begin( const vk::Pipeline *pipeline,
                                VkFramebuffer framebuffer,
                                algebra::matrix4f view ) {
    const VkCommandBufferBeginInfo beginInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = 0,
        .pInheritanceInfo = nullptr };
    // NOTE: omit return code check
    vkBeginCommandBuffer( commandBuffer_, &beginInfo );

    const VkRenderPassBeginInfo renderPassInfo{
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext = nullptr,
        .renderPass = pipeline->renderpass(),
        .framebuffer = framebuffer,
        .renderArea = { .offset = { .x = 0, .y = 0 },
                        .extent = { device_->extent() } },
        .clearValueCount = 1,
        .pClearValues = &clearColor_ };

    vkCmdBeginRenderPass( commandBuffer_, &renderPassInfo,
                          VK_SUBPASS_CONTENTS_INLINE );

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

    vkCmdBindPipeline( commandBuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS,
                       pipeline->pipeline() );
    vkCmdPushConstants( commandBuffer_, pipeline->layout(),
                        VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof( view ), &view );
}

void SceneRenderCommand::draw(
    const std::vector<VkBuffer> &vertexBuffer,
    const std::vector<VkDeviceSize> &verteciesOffsets,
    uint32_t verteciesCount ) {
    // VkBuffer vertexBuffers[] = { vertexBuffer };
    // VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers( commandBuffer_, 0, 1, vertexBuffer.data(),
                            verteciesOffsets.data() );
    vkCmdDraw( commandBuffer_, verteciesCount, 3, 0, 0 );
}

void SceneRenderCommand::end( VkSemaphore waitSemaphores,
                              VkSemaphore signalSemaphores, VkFence fence ) {
    vkCmdEndRenderPass( commandBuffer_ );

    // NOTE: omit return code check
    vkEndCommandBuffer( commandBuffer_ );

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

}  // namespace tire::vk
