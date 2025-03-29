
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
    const auto [iaSem, rfSem, ifFnc] =
        context_->getFrameSyncSet( currentFrame_ );

    // NOTE: omit return code check
    vkWaitForFences( context_->device(), 1, &ifFnc, VK_TRUE, UINT64_MAX );

    // NOTE: omit return code check
    vkResetFences( context_->device(), 1, &ifFnc );

    // NOTE: omit return code check
    // May return VK_SUBOPTIMAL_KHR or even VK_ERROR_OUT_OF_DATE_KHR
    // if current surface properties are no longer matched
    // exactly or swap chain has become incompatible
    // with the surface and can no longer be used for rendering
    uint32_t imageIndex{};
    vkAcquireNextImageKHR( context_->device(), context_->swapchain(),
                           UINT64_MAX, iaSem, VK_NULL_HANDLE, &imageIndex );

    // NOTE: currentFrame_->imageIndex
    const auto currentFramebuffer = context_->framebuffer( currentFrame_ );

    auto handle = static_cast<vk::Scene*>( scene_.get() );
    handle->output( currentFramebuffer, currentFrame_, iaSem, rfSem, ifFnc );

    context_->present( rfSem, &currentFrame_ );

    currentFrame_ = ( currentFrame_ + 1 ) % context_->framesCount();
};

void RenderVK::postFrame() {

};

void RenderVK::swapBuffers() {

};

void RenderVK::postLoop() {
    log::info( "vk::Render === wait device idle..." );

    // we should wait for the logical device to finish operations
    // before exiting mainLoop and destroying the window
    vkDeviceWaitIdle( context_->device() );

    scene_->clean();
};

}  // namespace tire
