
#include <vulkan/vulkan.h>

#include "rendervk.h"

namespace tire {

void RenderVK::preLoop(){

};

void RenderVK::preFrame() {
    vkWaitForFences( device_, 1, &inFlightFence_, VK_TRUE, UINT64_MAX );

    uint32_t imageIndex;
    vkAcquireNextImageKHR( device_, swapChain_, UINT64_MAX,
                           imageAvailableSemaphore_, VK_NULL_HANDLE,
                           &imageIndex );

    cBuf_->reset();
    cBuf_->beginRenderPassCommand( framebuffers_[imageIndex],
                                   pipelineSimple_.get() );
    cBuf_->submit( { imageAvailableSemaphore_ }, { renderFinishedSemaphore_ },
                   inFlightFence_, graphicsQueue_ );

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    VkSemaphore signalSemaphores[] = { renderFinishedSemaphore_ };
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { swapChain_ };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    vkQueuePresentKHR( presentQueue_, &presentInfo );
};

void RenderVK::frame(){

};

void RenderVK::postFrame() {
    vkResetFences( device_, 1, &inFlightFence_ );
};

void RenderVK::swapBuffers(){

};

void RenderVK::postLoop() {
    // we should wait for the logical device to finish operations
    // before exiting mainLoop and destroying the window
    vkDeviceWaitIdle( device_ );
};

}  // namespace tire
