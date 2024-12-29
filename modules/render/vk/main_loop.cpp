
#include <vulkan/vulkan.h>

#include "rendervk.h"

namespace tire {

void RenderVK::initMainLoop(){

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
};

void RenderVK::frame(){

};

void RenderVK::postFrame() {
    vkResetFences( device_, 1, &inFlightFence_ );
};

void RenderVK::swapBuffers(){

};

}  // namespace tire
