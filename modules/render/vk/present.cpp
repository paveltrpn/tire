
#include <vulkan/vk_enum_string_helper.h>

#include "present.h"
#include "log/log.h"

namespace tire::vk {

Present::Present( const vk::Device *device, const vk::Swapchain *swapchain )
    : device_{ device }
    , swapchain_{ swapchain } {
    presentInfo_.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo_.waitSemaphoreCount = 1;
    presentInfo_.pResults = nullptr;
}

void Present::present( const VkSemaphore semaphore, uint32_t *imageIndex ) {
    VkSwapchainKHR swapChains[] = { swapchain_->handle() };
    presentInfo_.swapchainCount = 1;
    presentInfo_.pSwapchains = swapChains;

    VkSemaphore signalSemaphores[] = { semaphore };
    presentInfo_.pWaitSemaphores = signalSemaphores;
    presentInfo_.pImageIndices = imageIndex;

    if ( const auto res =
             vkQueuePresentKHR( device_->presentQueue(), &presentInfo_ );
         res != VK_SUCCESS ) {
        log::warning( "queue present result {}, is it normal?",
                      string_VkResult( res ) );
    }
}

}  // namespace tire::vk
