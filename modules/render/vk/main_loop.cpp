
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#include "rendervk.h"

namespace tire {

void RenderVK::preLoop(){

};

void RenderVK::preFrame(){

};

void RenderVK::frame() {
    //log::debug<true>( "currentFrame = {}", currentFrame_ );

    // cause deadlock
#define ONE_SECOND 1000000000
    const auto waitFenceResult =
        vkWaitForFences( device_->handle(), 1, &inFlightFences_[currentFrame_],
                         VK_TRUE, ONE_SECOND );
    // log::debug<true>( "wait fences result = {}",
    //                   string_VkResult( waitFenceResult ) );

    vkResetFences( device_->handle(), 1, &inFlightFences_[currentFrame_] );

    uint32_t imageIndex;
    const auto result = vkAcquireNextImageKHR(
        device_->handle(), swapchain_->handle(), UINT64_MAX,
        imageAvailableSemaphores_[currentFrame_], VK_NULL_HANDLE, &imageIndex );
    if ( result == VK_ERROR_OUT_OF_DATE_KHR ) {
        return;
    } else if ( result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR ) {
        throw std::runtime_error( "failed to acquire swap chain image!" );
    }

    cBufs_[currentFrame_]->reset();
    cBufs_[currentFrame_]->beginRenderPassCommand(
        swapchain_->framebuffer( imageIndex ), pipelineSimple_.get() );
    cBufs_[currentFrame_]->submit( { imageAvailableSemaphores_[currentFrame_] },
                                   { renderFinishedSemaphores_[currentFrame_] },
                                   inFlightFences_[currentFrame_],
                                   device_->graphicsQueue() );
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    VkSemaphore signalSemaphores[] = {
        renderFinishedSemaphores_[currentFrame_] };
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { swapchain_->handle() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    vkQueuePresentKHR( device_->presentQueue(), &presentInfo );
    vkResetFences( device_->handle(), 1, &inFlightFences_[currentFrame_] );

    currentFrame_ = ( currentFrame_ + 1 ) % 2;
};

void RenderVK::postFrame(){

};

void RenderVK::swapBuffers(){

};

void RenderVK::postLoop() {
    // we should wait for the logical device to finish operations
    // before exiting mainLoop and destroying the window
    vkDeviceWaitIdle( device_->handle() );
};

}  // namespace tire
