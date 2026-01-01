
module;

#include <cstdint>
#include <format>

#include <vulkan/vk_enum_string_helper.h>

static constexpr bool DEBUG_OUTPUT_COMMAND_BUFFER_CPP{ true };

export module context:command_buffer;

import log;

import :context;

namespace tire {

auto Context::renderCommandBegin( uint32_t frameId ) -> void {
    const auto [iaSem, rfSem, ifFnc, cb] = getFrameSyncSet( frameId );

    std::array<VkFence, 1> fences = { ifFnc };

    // NOTE: omit return code check
    vkWaitForFences( device(), fences.size(), fences.data(), VK_TRUE, UINT64_MAX );

    // NOTE: omit return code check
    vkResetFences( device(), fences.size(), fences.data() );

    vkResetCommandBuffer( cb, 0 );

    // NOTE: omit return code check
    // May return VK_SUBOPTIMAL_KHR or even VK_ERROR_OUT_OF_DATE_KHR
    // if current surface properties are no longer matched
    // exactly or swap chain has become incompatible
    // with the surface and can no longer be used for rendering
    uint32_t imageIndex{};
    vkAcquireNextImageKHR( device(), swapchain(), UINT64_MAX, iaSem, VK_NULL_HANDLE, &imageIndex );

    const VkCommandBufferBeginInfo beginInfo{
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, .flags = 0, .pInheritanceInfo = nullptr };

    vkBeginCommandBuffer( cb, &beginInfo );

    const auto currentFramebuffer = framebuffer( frameId );

    const auto [viewportWidth, viewportHeight] = viewportSize();

    const VkRenderPassBeginInfo renderPassInfo{
      .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
      .pNext = nullptr,
      .renderPass = renderPass(),
      .framebuffer = currentFramebuffer,
      .renderArea = { .offset = { .x = 0, .y = 0 }, .extent = { viewportWidth, viewportHeight } },
      .clearValueCount = static_cast<uint32_t>( clearValues_.size() ),
      .pClearValues = clearValues_.data() };

    vkCmdBeginRenderPass( cb, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE );
}

auto Context::renderCommandEnd( uint32_t frameId ) -> void {
    const auto [iaSem, rfSem, ifFnc, cb] = getFrameSyncSet( frameId );

    vkCmdEndRenderPass( cb );
    // NOTE: omit return code check
    vkEndCommandBuffer( cb );

    std::array<VkPipelineStageFlags, 1> waitStages = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    std::array<VkSemaphore, 1> waitsems{ iaSem };
    std::array<VkSemaphore, 1> sgnlsems{ rfSem };
    std::array<VkCommandBuffer, 1> commands{ cb };

    const VkSubmitInfo submitInfo{
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .waitSemaphoreCount = static_cast<uint32_t>( waitsems.size() ),
      .pWaitSemaphores = waitsems.data(),
      .pWaitDstStageMask = waitStages.data(),
      .commandBufferCount = static_cast<uint32_t>( commands.size() ),
      .pCommandBuffers = commands.data(),
      .signalSemaphoreCount = static_cast<uint32_t>( sgnlsems.size() ),
      .pSignalSemaphores = sgnlsems.data() };

    // NOTE: PRESET!
    // NOTE: omit return code check
    vkQueueSubmit( graphicsQueue(), static_cast<uint32_t>( commands.size() ), &submitInfo, ifFnc );

    std::array<VkSwapchainKHR, 1> swapChains = { swapchain_ };
    std::array<VkSemaphore, 1> signalSemaphores = { rfSem };
    std::array<uint32_t, 1> imageIds = { frameId };

    const VkPresentInfoKHR presentInfo{
      .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
      .pNext = nullptr,
      .waitSemaphoreCount = 1,
      .pWaitSemaphores = signalSemaphores.data(),
      .swapchainCount = 1,
      .pSwapchains = swapChains.data(),
      .pImageIndices = imageIds.data(),
      .pResults = nullptr };

    // May return VK_SUBOPTIMAL_KHR or even VK_ERROR_OUT_OF_DATE_KHR
    // if current surface properties are no longer matched
    // exactly or swap chain has become incompatible
    // with the surface and can no longer be used for rendering
    if ( const auto res = vkQueuePresentKHR( presentQueue_, &presentInfo ); res != VK_SUCCESS ) {
        log::warning( "queue present result {}, is it normal?", string_VkResult( res ) );
    }
}

}  // namespace tire
