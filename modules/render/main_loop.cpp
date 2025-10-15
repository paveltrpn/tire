
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#include "rendervk.h"

namespace tire {

void RenderVK::preLoop() {
    log::notice( "vk::Render === render loop starts here..." );
};

void RenderVK::preFrame() {
    // Update global timer
    timer_.update();

    // Update scene objects
    scene_->traverse( timer_.frameDuration<float>() );

    scene_->camera().traverse();

    scene_->submit();
};

void RenderVK::frame( VkCommandBuffer cb ) {
    // auto handle = static_cast<vk::Scene*>( scene_.get() );
    // handle->output( cb );
    drawTestCube( cb );
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
