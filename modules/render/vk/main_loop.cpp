
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <span>

#include "rendervk.h"

import event;

namespace tire {

void RenderVK::preLoop() {
    log::notice( "render loop starts here..." );

    auto handle = showMetrics();
    handle.scheduleDestroy();

    event::testTimerEvent( 1000 );
    executeByTimeOut( 1500 );
    watchFile( "/mnt/main_disk/code/tiny_render/assets/figures.json" );
};

void RenderVK::preFrame() {
    // Update global timer
    timer_.update();
    // Update scene objects
    scene_->traverse( timer_.floatFrameDuration() );
    scene_->submit();
};

void RenderVK::frame() {
    uint32_t imageIndex{};

    const auto [iaSem, rfSem, ifFnc] =
        context_->getFrameSyncSet( currentFrame_ );

#define ONE_SECOND 1000000000
    // NOTE: omit return code check
    vkWaitForFences( context_->device(), 1, &ifFnc, VK_TRUE, UINT64_MAX );

    // NOTE: omit return code check
    vkResetFences( context_->device(), 1, &ifFnc );

    // NOTE: omit return code check
    // May return VK_SUBOPTIMAL_KHR or even VK_ERROR_OUT_OF_DATE_KHR
    // if current surface properties are no longer matched
    // exactly or swap chain has become incompatible
    // with the surface and can no longer be used for rendering
    vkAcquireNextImageKHR( context_->device(), context_->swapchain(),
                           UINT64_MAX, iaSem, VK_NULL_HANDLE, &imageIndex );

    // NOTE: currentFrame_->imageIndex
    const auto currentFramebuffer = context_->framebuffer( currentFrame_ );

    auto handle = static_cast<vk::Scene*>( scene_.get() );
    handle->output( currentFramebuffer, currentFrame_, iaSem, rfSem, ifFnc );

    context_->present( rfSem, &currentFrame_ );

    // TODO: decide is correct to use imageIndex from vkAcquireNextImageKHR()
    // instead if currentFrame? Maybe it works because of number of swapchain
    // images is equal to frames in flight? What will happen if this values be different?
    currentFrame_ = ( currentFrame_ + 1 ) % context_->framesCount();
};

void RenderVK::postFrame() {

};

void RenderVK::swapBuffers() {

};

void RenderVK::postLoop() {
    // we should wait for the logical device to finish operations
    // before exiting mainLoop and destroying the window
    vkDeviceWaitIdle( context_->device() );

    scene_->clean();
};

}  // namespace tire
