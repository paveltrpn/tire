
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#include "rendervk.h"

namespace tire {

void RenderVK::preLoop(){

};

void RenderVK::preFrame(){

};

void RenderVK::frame() {
    const auto [iaSem, rfSem, ifFnc] = presentSync_->get( currentFrame_ );

#define ONE_SECOND 1000000000
    // NOTE: omit return code check
    vkWaitForFences( device_->handle(), 1, &ifFnc, VK_TRUE, 0 );

    // NOTE: omit return code check
    vkResetFences( device_->handle(), 1, &ifFnc );

    uint32_t imageIndex{};

    // NOTE: omit return code check
    vkAcquireNextImageKHR( device_->handle(), swapchain_->handle(), UINT64_MAX,
                           iaSem, VK_NULL_HANDLE, &imageIndex );

    cBufs_[currentFrame_]->reset();
    cBufs_[currentFrame_]->prepare( swapchain_->framebuffer( imageIndex ),
                                    pipelineSimple_.get() );
    cBufs_[currentFrame_]->submit( { iaSem }, { rfSem }, ifFnc );

    /*
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
    */
    present_->present( rfSem, &imageIndex );

    // NOTE: omit return code check
    vkResetFences( device_->handle(), 1, &ifFnc );

    currentFrame_ = ( currentFrame_ + 1 ) % 3;
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
