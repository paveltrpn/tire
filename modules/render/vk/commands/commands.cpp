
#include <vulkan/vk_enum_string_helper.h>

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_COMMAND_POOL_CPP{ true };

#include "image/color.h"
#include "commands.h"
#include "algebra/matrix4.h"

namespace tire::vk {

// ==========================================================================

struct TransformPushConstants {
    algebra::matrix4f view;
    algebra::matrix4f rtn;
};

void RenderFromShader::reset() {
    vkResetCommandBuffer( commandBuffer_, 0 );
}

void RenderFromShader::prepare( VkFramebuffer framebuffer,
                                const vk::Pipeline *pipeline ) {
    const VkCommandBufferBeginInfo beginInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = 0,
        .pInheritanceInfo = nullptr };
    // NOTE: omit return code check
    vkBeginCommandBuffer( commandBuffer_, &beginInfo );

    const auto clearColor = Colorf{ "darkblue" };
    const VkClearValue clearColorValue = {
        { { clearColor.r(), clearColor.g(), clearColor.b(),
            clearColor.a() } } };

    const VkRenderPassBeginInfo renderPassInfo{
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext = nullptr,
        .renderPass = pipeline->renderpass(),
        .framebuffer = framebuffer,
        .renderArea = { .offset = { .x = 0, .y = 0 },
                        .extent = { device_->extent() } },
        .clearValueCount = 1,
        .pClearValues = &clearColorValue };

    vkCmdBeginRenderPass( commandBuffer_, &renderPassInfo,
                          VK_SUBPASS_CONTENTS_INLINE );

    vkCmdBindPipeline( commandBuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS,
                       pipeline->pipeline() );

    // Dynamic viewport. No performance penalty.
    // Take out work from pipeline creation.
    const auto width = device_->extent().width;
    const auto height = device_->extent().height;

    const VkViewport viewport{ .x = 0.0f,
                               .y = 0.0f,
                               .width = static_cast<float>( width ),
                               .height = static_cast<float>( height ),
                               .minDepth = 0.0f,
                               .maxDepth = 1.0f };
    vkCmdSetViewport( commandBuffer_, 0, 1, &viewport );

    const VkRect2D scissor{ { .x = 0, .y = 0 },
                            { .width = width, .height = height } };
    vkCmdSetScissor( commandBuffer_, 0, 1, &scissor );

    static float factor{ 0.0f };
    auto projection = algebra::perspective<float>(
        45.0f, static_cast<float>( width ) / static_cast<float>( height ), 0.01,
        100.0 );
    auto offset = algebra::translate<float>( 0.0f, 0.f, -2.0f );
    offset.transposeSelf();
    TransformPushConstants transform{};
    transform.view = offset * projection;
    // transform.view[1, 1] *= -1;
    transform.rtn = algebra::rotate<float>( factor, 40, 9 );
    factor += 0.3;
    vkCmdPushConstants( commandBuffer_, pipeline->layout(),
                        VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof( transform ),
                        &transform );

    vkCmdDraw( commandBuffer_, verteciesCount_, 3, 0, 0 );

    vkCmdEndRenderPass( commandBuffer_ );

    // NOTE: omit return code check
    vkEndCommandBuffer( commandBuffer_ );
}

void RenderFromShader::submit( const std::vector<VkSemaphore> &waitSemaphores,
                               const std::vector<VkSemaphore> &signalSemaphores,
                               VkFence fence ) {
    std::array<VkPipelineStageFlags, 1> waitStages = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    VkSubmitInfo submitInfo{ .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                             .waitSemaphoreCount = 1,
                             .pWaitSemaphores = waitSemaphores.data(),
                             .pWaitDstStageMask = waitStages.data(),
                             .commandBufferCount = 1,
                             .pCommandBuffers = &commandBuffer_,
                             .signalSemaphoreCount = 1,
                             .pSignalSemaphores = signalSemaphores.data() };

    // NOTE: omit return code check
    vkQueueSubmit( device_->graphicsQueue(), 1, &submitInfo, fence );
}

}  // namespace tire::vk
