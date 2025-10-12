
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <span>

#include "rendervk.h"

import event;

namespace tire {

void RenderVK::preLoop() {
    log::notice( "render loop starts here..." );

    // auto handle = showMetrics();
    // handle.scheduleDestroy();

    event::testTimerEvent( 1000 );
    //executeByTimeOut( 1500 );
    //watchFile( "/mnt/main_disk/code/tiny_render/assets/figures.json" );
};

void RenderVK::preFrame() {
    // Update global timer
    timer_.update();

    // Update scene objects
    scene_->traverse( timer_.frameDuration<float>() );

    scene_->camera().traverse();

    scene_->submit();
};

void RenderVK::frame() {
    context_->renderCommandBegin( currentFrame_ );

    const auto currentFramebuffer = context_->framebuffer( currentFrame_ );

    auto handle = static_cast<vk::Scene*>( scene_.get() );
    handle->output( context_->getDrawCommandBuffer(), currentFramebuffer );

    context_->renderCommandEnd( currentFrame_ );

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
