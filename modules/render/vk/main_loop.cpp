
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <span>

#include "rendervk.h"
#include "event/timer.h"

namespace tire {

void RenderVK::preLoop() {
    log::notice( "render loop starts here..." );
    event::testTimerEvent( 2000 );
};

void RenderVK::preFrame(){

};

void RenderVK::frame() {
    uint32_t imageIndex{};

    const auto [iaSem, rfSem, ifFnc] = presentSync_->get( imageIndex );

#define ONE_SECOND 1000000000
    // NOTE: omit return code check
    vkWaitForFences( device_->handle(), 1, &ifFnc, VK_TRUE, 0 );

    // NOTE: omit return code check
    vkResetFences( device_->handle(), 1, &ifFnc );

    // NOTE: omit return code check
    vkAcquireNextImageKHR( device_->handle(), swapchain_->handle(), UINT64_MAX,
                           iaSem, VK_NULL_HANDLE, &imageIndex );

    // NOTE: currentFrame_->imageIndex
    const auto currentFramebuffer = swapchain_->framebuffer( imageIndex );
    cBufs_[imageIndex]->setProperties( currentFramebuffer,
                                       piplineMatrixReady.get(), 36 );
    cBufs_[imageIndex]->reset();
    cBufs_[imageIndex]->bind();
    cBufs_[imageIndex]->submit( iaSem, rfSem, ifFnc );

    present_->present( rfSem, &imageIndex );

    // NOTE: omit return code check
    vkResetFences( device_->handle(), 1, &ifFnc );

    // TODO: decide is correct to use imageIndex from vkAcquireNextImageKHR()
    // instead if currentFrame? Maybe it works because of number of swapchain
    // images is equal to frames in flight? What will happen if this values be different?
    currentFrame_ = ( currentFrame_ + 1 ) % FRAMES_IN_FLIGHT_COUNT;
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
