
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

    const auto [iaSem, rfSem, ifFnc] = presentSync_->get( currentFrame_ );

#define ONE_SECOND 1000000000
    // NOTE: omit return code check
    vkWaitForFences( device_->handle(), 1, &ifFnc, VK_TRUE, 0 );

    // NOTE: omit return code check
    vkResetFences( device_->handle(), 1, &ifFnc );

    // NOTE: omit return code check
    vkAcquireNextImageKHR( device_->handle(), swapchain_->handle(), UINT64_MAX,
                           iaSem, VK_NULL_HANDLE, &imageIndex );
    const auto currentFramebuffer = swapchain_->framebuffer( imageIndex );

    cBufs_[currentFrame_]->setProperties( currentFramebuffer,
                                          piplineMatrixReady.get(), 36 );
    cBufs_[currentFrame_]->reset();
    cBufs_[currentFrame_]->bind();
    cBufs_[currentFrame_]->submit( iaSem, rfSem, ifFnc );

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
