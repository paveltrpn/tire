
#include <format>

#include <vulkan/vk_enum_string_helper.h>

#include "context_bare.h"

#include "image/color.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_COMMAND_BUFFER_CPP{ true };

namespace tire::vk {

auto ContextBare::initPrimaryCommandBuffer() -> void {
    const VkCommandBufferAllocateInfo allocInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = commandPool(),
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1 };
    VkCommandBuffer buffer{ VK_NULL_HANDLE };
    const auto err =
        vkAllocateCommandBuffers( device(), &allocInfo, &cbPrimary_ );
    if ( err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to allocate command buffers with code {}!",
                         string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_COMMAND_BUFFER_CPP>(
            "vk::Context === primary command buffer created!" );
    };

    // TODO: pass background color fdrom scene environment parameters.
    // Note that the order of clearValues should be identical to the order of your
    // attachments
    const auto backgroundColor = Colorf( "#0f0f0f" );
    clearValues_[0].color = { { backgroundColor.r(), backgroundColor.g(),
                                backgroundColor.b(), 1.0f } };
    clearValues_[1].depthStencil = { .depth = 0.0f, .stencil = 0 };
}

auto ContextBare::initSecondaryCommandBuffer() -> void {
}

auto ContextBare::renderCommandBegin( uint32_t frameId,
                                      VkRenderPass renderPass ) -> void {
    const auto [iaSem, rfSem, ifFnc] = getFrameSyncSet( frameId );

    vkResetCommandBuffer( cbPrimary_, 0 );

    // NOTE: omit return code check
    vkWaitForFences( device(), 1, &ifFnc, VK_TRUE, UINT64_MAX );

    // NOTE: omit return code check
    vkResetFences( device(), 1, &ifFnc );

    // NOTE: omit return code check
    // May return VK_SUBOPTIMAL_KHR or even VK_ERROR_OUT_OF_DATE_KHR
    // if current surface properties are no longer matched
    // exactly or swap chain has become incompatible
    // with the surface and can no longer be used for rendering
    uint32_t imageIndex{};
    vkAcquireNextImageKHR( device(), swapchain(), UINT64_MAX, iaSem,
                           VK_NULL_HANDLE, &imageIndex );

    const VkCommandBufferBeginInfo beginInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = 0,
        .pInheritanceInfo = nullptr };
    // NOTE: omit return code check
    vkBeginCommandBuffer( cbPrimary_, &beginInfo );

    const auto currentFramebuffer = framebuffer( frameId );

    const VkRenderPassBeginInfo renderPassInfo{
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .pNext = nullptr,
        .renderPass = renderPass,
        .framebuffer = currentFramebuffer,
        .renderArea = { .offset = { .x = 0, .y = 0 },
                        .extent = { currentExtent() } },
        .clearValueCount = static_cast<uint32_t>( clearValues_.size() ),
        .pClearValues = clearValues_.data() };

    vkCmdBeginRenderPass( cbPrimary_, &renderPassInfo,
                          VK_SUBPASS_CONTENTS_INLINE );
}

auto ContextBare::renderCommandEnd( uint32_t frameId ) -> void {
    const auto [iaSem, rfSem, ifFnc] = getFrameSyncSet( frameId );

    vkCmdEndRenderPass( cbPrimary_ );
    // NOTE: omit return code check
    vkEndCommandBuffer( cbPrimary_ );
    std::array<VkPipelineStageFlags, 1> waitStages = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    std::array<VkSemaphore, 1> waitsems{ iaSem };
    std::array<VkSemaphore, 1> sgnlsems{ rfSem };

    std::array<VkCommandBuffer, 1> commands{ cbPrimary_ };
    const VkSubmitInfo submitInfo{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = waitsems.data(),
        .pWaitDstStageMask = waitStages.data(),
        .commandBufferCount = static_cast<uint32_t>( commands.size() ),
        .pCommandBuffers = commands.data(),
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = sgnlsems.data() };

    // NOTE: PRESET!
    // NOTE: omit return code check
    vkQueueSubmit( graphicsQueue(), static_cast<uint32_t>( commands.size() ),
                   &submitInfo, ifFnc );

    std::array<VkSwapchainKHR, 1> swapChains = { swapchain_ };
    std::array<VkSemaphore, 1> signalSemaphores = { rfSem };

    const VkPresentInfoKHR presentInfo{
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = signalSemaphores.data(),
        .swapchainCount = 1,
        .pSwapchains = swapChains.data(),
        .pImageIndices = &frameId,
        .pResults = nullptr };

    // May return VK_SUBOPTIMAL_KHR or even VK_ERROR_OUT_OF_DATE_KHR
    // if current surface properties are no longer matched
    // exactly or swap chain has become incompatible
    // with the surface and can no longer be used for rendering
    if ( const auto res = vkQueuePresentKHR( presentQueue_, &presentInfo );
         res != VK_SUCCESS ) {
        log::warning( "queue present result {}, is it normal?",
                      string_VkResult( res ) );
    }
}

}  // namespace tire::vk