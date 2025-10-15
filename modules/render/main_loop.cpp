
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
    const auto [width, height] = context_->viewportSize();

    // Dynamic viewport. No performance penalty.
    // Take out work from pipeline creation.
    // NOTE: Define negative viewport size to use same projection matrix as
    // for OpenGL pipeline.
    const VkViewport viewport{ .x = 0.0f,
                               .y = static_cast<float>( height ),
                               .width = static_cast<float>( width ),
                               .height = -static_cast<float>( height ),
                               .minDepth = 0.0f,
                               .maxDepth = 1.0f };
    // const VkViewport viewport{ .x = 0.0f,
    //    .y = 0.0f,
    //    .width = static_cast<float>( width ),
    //    .height = static_cast<float>( height ),
    //    .minDepth = 0.0f,
    //                         .maxDepth = 1.0f };
    vkCmdSetViewport( cb, 0, 1, &viewport );

    const VkRect2D scissor{ { .x = 0, .y = 0 },
                            { .width = width, .height = height } };
    vkCmdSetScissor( cb, 0, 1, &scissor );

    auto handle = static_cast<vk::Scene*>( scene_.get() );
    handle->output( cb );
    // drawTestCube( cb );
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
