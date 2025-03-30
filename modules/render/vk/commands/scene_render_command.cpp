
#include <vulkan/vk_enum_string_helper.h>

#include "scene_render_command.h"

#include "algebra/matrix4.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_SCENE_RENDER_COMMAND_CPP{ true };

namespace tire::vk {

SceneRenderCommand::SceneRenderCommand( const vk::Context *context,
                                        const vk::Pipeline *pipeline )
    : context_{ context }
    , pipeline_{ pipeline } {
    const VkCommandBufferAllocateInfo allocInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = context_->commandPool(),
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1 };
    VkCommandBuffer buffer{ VK_NULL_HANDLE };
    const auto err =
        vkAllocateCommandBuffers( context_->device(), &allocInfo, &command_ );
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

    width_ = context_->currentExtent().width;
    height_ = context_->currentExtent().height;
}

void SceneRenderCommand::clean() {
    // Because command buffers are freed for us when we free the command
    // pool, there is nothing extra to do for command buffer cleanup.
    // vkFreeCommandBuffers( context_->device(), context_->commandPool(), 1,
    //   &command_ );
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
                        .extent = { context_->currentExtent() } },
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

void SceneRenderCommand::bindBuffer( Colorf color, VkBuffer vbo, VkBuffer nbo,
                                     uint32_t vCount ) {
    const auto c = color.asVector4f();
    vkCmdPushConstants( command_, pipeline_->layout(),
                        VK_SHADER_STAGE_VERTEX_BIT, sizeof( algebra::matrix4f ),
                        sizeof( color ), &c );

    // NOTE: see https://docs.vulkan.org/guide/latest/vertex_input_data_processing.html
    std::array<VkBuffer, 2> vertexBuffers = { vbo, nbo };
    std::array<VkDeviceSize, 2> offsets = { 0, 0 };
    vkCmdBindVertexBuffers( command_, 0, 2, vertexBuffers.data(),
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
    vkQueueSubmit( context_->graphicsQueue(),
                   static_cast<uint32_t>( commands.size() ), &submitInfo,
                   fence );
}

}  // namespace tire::vk
