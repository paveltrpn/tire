
#include <vulkan/vk_enum_string_helper.h>

#include "present.h"
#include "log/log.h"

namespace tire::vk {

Present::Present( const vk::Device *device, const vk::Swapchain *swapchain )
    : device_{ device }
    , swapchain_{ swapchain } {
}

void Present::present( const VkSemaphore semaphore, uint32_t *imageIndex ) {
    std::array<VkSwapchainKHR, 1> swapChains = { swapchain_->handle() };
    std::array<VkSemaphore, 1> signalSemaphores = { semaphore };

    const VkPresentInfoKHR presentInfo{
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = signalSemaphores.data(),
        .swapchainCount = 1,
        .pSwapchains = swapChains.data(),
        .pImageIndices = imageIndex,
        .pResults = nullptr };

    // May return VK_SUBOPTIMAL_KHR or even VK_ERROR_OUT_OF_DATE_KHR
    // if current surface properties are no longer matched
    // exactly or swap chain has become incompatible
    // with the surface and can no longer be used for rendering
    if ( const auto res =
             vkQueuePresentKHR( device_->presentQueue(), &presentInfo );
         res != VK_SUCCESS ) {
        log::warning( "queue present result {}, is it normal?",
                      string_VkResult( res ) );
    }
}

}  // namespace tire::vk
