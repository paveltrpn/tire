
#include "scene_render_command.h"

#include "algebra/matrix4.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_SCENE_RENDER_COMMAND_CPP{ true };

namespace tire::vk {

SceneRenderCommand::SceneRenderCommand( const vk::Device *device,
                                        const vk::Pipeline *pipeline,
                                        const CommandPool *pool,
                                        uint32_t commandsCount )
    : device_{ device }
    , pipeline_{ pipeline }
    , pool_{ pool } {
    const VkCommandBufferAllocateInfo allocInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = pool_->handle(),
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1 };
    VkCommandBuffer buffer{ VK_NULL_HANDLE };
    const auto err =
        vkAllocateCommandBuffers( device_->handle(), &allocInfo, &command_ );
    if ( err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to allocate command buffers with code {}!",
                         string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_SCENE_RENDER_COMMAND_CPP>(
            "vk::SceneRenderCommand === buffer created!" );
    };

    // Note that the order of clearValues should be identical to the order of your
    // attachments
    const auto backgroundColor = Colorf( "#0f0f0f" );
    clearValues_[0].color = { { backgroundColor.r(), backgroundColor.g(),
                                backgroundColor.b(), 1.0f } };
    clearValues_[1].depthStencil = { .depth = 0.0f, .stencil = 0 };

    width_ = device_->extent().width;
    height_ = device_->extent().height;
}

void SceneRenderCommand::clean() {
    vkFreeCommandBuffers( device_->handle(), pool_->handle(), 1, &command_ );
}

void SceneRenderCommand::reset() {
    vkResetCommandBuffer( command_, 0 );
}

void SceneRenderCommand::prepare( VkFramebuffer framebuffer,
                                  algebra::matrix4f view ) {
    const VkCommandBufferBeginInfo beginInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = 0,
        .pInheritanceInfo = nullptr };
    // NOTE: omit return code check
    vkBeginCommandBuffer( command_, &beginInfo );

    const VkRenderPassBeginInfo renderPassInfo{
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext = nullptr,
        .renderPass = pipeline_->renderpass(),
        .framebuffer = framebuffer,
        .renderArea = { .offset = { .x = 0, .y = 0 },
                        .extent = { device_->extent() } },
        .clearValueCount = static_cast<uint32_t>( clearValues_.size() ),
        .pClearValues = clearValues_.data() };

    vkCmdBeginRenderPass( command_, &renderPassInfo,
                          VK_SUBPASS_CONTENTS_INLINE );

    // Dynamic viewport. No performance penalty.
    // Take out work from pipeline creation.
    // NOTE: Define negative viewport size to use same projection matrix as
    // for OpenGL pipeline.
    // const VkViewport viewport{ .x = 0.0f,
    //    .y = static_cast<float>( height_ ),
    //    .width = static_cast<float>( width_ ),
    //    .height = -static_cast<float>( height_ ),
    //    .minDepth = 0.0f,
    //    .maxDepth = 1.0f };
    const VkViewport viewport{ .x = 0.0f,
                               .y = 0.0f,
                               .width = static_cast<float>( width_ ),
                               .height = static_cast<float>( height_ ),
                               .minDepth = 0.0f,
                               .maxDepth = 1.0f };
    vkCmdSetViewport( command_, 0, 1, &viewport );

    const VkRect2D scissor{ { .x = 0, .y = 0 },
                            { .width = width_, .height = height_ } };
    vkCmdSetScissor( command_, 0, 1, &scissor );

    vkCmdBindPipeline( command_, VK_PIPELINE_BIND_POINT_GRAPHICS,
                       pipeline_->pipeline() );
    vkCmdPushConstants( command_, pipeline_->layout(),
                        VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof( view ), &view );
}

void SceneRenderCommand::bindBuffer( algebra::vector3f color, VkBuffer vbo,
                                     uint32_t vCount ) {
    vkCmdPushConstants( command_, pipeline_->layout(),
                        VK_SHADER_STAGE_VERTEX_BIT, sizeof( float ) * 16,
                        sizeof( color ), &color );

    std::array<VkBuffer, 1> vertexBuffers = { vbo };
    std::array<VkDeviceSize, 1> offsets = { 0 };
    vkCmdBindVertexBuffers( command_, 0, 1, vertexBuffers.data(),
                            offsets.data() );
    vkCmdDraw( command_, vCount, 3, 0, 0 );
}

void SceneRenderCommand::submit( VkSemaphore waitSemaphores,
                                 VkSemaphore signalSemaphores, VkFence fence ) {
    vkCmdEndRenderPass( command_ );
    // NOTE: omit return code check
    vkEndCommandBuffer( command_ );
    std::array<VkPipelineStageFlags, 1> waitStages = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    std::array<VkSemaphore, 1> waitsems{ waitSemaphores };
    std::array<VkSemaphore, 1> sgnlsems{ signalSemaphores };

    std::array<VkCommandBuffer, 1> commands{ command_ };
    const VkSubmitInfo submitInfo{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = waitsems.data(),
        .pWaitDstStageMask = waitStages.data(),
        .commandBufferCount = static_cast<uint32_t>( commands.size() ),
        .pCommandBuffers = commands.data(),
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = sgnlsems.data() };

    // NOTE: omit return code check
    vkQueueSubmit( device_->graphicsQueue(),
                   static_cast<uint32_t>( commands.size() ), &submitInfo,
                   fence );
}

}  // namespace tire::vk
