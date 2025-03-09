
#include <vector>

#include "algebra/vector4.h"
#include "render/vk/commands/scene_render_command.h"
#include "rendervk.h"
#include "scene/camera.h"
#include "vulkan/vulkan_core.h"
#include "scene.h"

namespace tire::vk {

Scene::Scene( const std::filesystem::path &fname, const vk::Device *device,
              const vk::Pipeline *pipeline )
    : tire::Scene{ fname }
    , device_{ device }
    , pipeline_{ pipeline } {
    commandPool_ = std::make_unique<vk::CommandPool>( device_ );

    // Create scene draw command (three times)
    cBufs_.reserve( FRAMES_IN_FLIGHT_COUNT );
    for ( auto i = 0; i < cBufs_.capacity(); ++i ) {
        cBufs_.push_back( std::make_unique<vk::SceneRenderCommand>(
            device_, pipeline_, commandPool_.get(), bodyList_.size() ) );
    }

    // Create vulkan "vertex buffers"
    const auto nodeListSize = bodyList_.size();
    buffersList_.reserve( nodeListSize );
    for ( size_t i{}; i < nodeListSize; ++i ) {
        auto buf = std::make_shared<vk::VertexBuffer<float>>(
            device_, bodyList_[i]->verteciesCount() );
        buffersList_.push_back( std::move( buf ) );
    }
}

void Scene::submit() {
    // Update data in vulkan "vertex" buffers, i.e. copy from CPU memory
    for ( size_t i = 0; auto &buffer : buffersList_ ) {
        buffer->populate(
            reinterpret_cast<const void *>( bodyList_[i]->verteciesData() ) );
        ++i;
    }
}

void Scene::output( const VkFramebuffer currentFramebuffer, uint32_t imageIndex,
                    VkSemaphore waitSemaphore, VkSemaphore signalSemaphore,
                    VkFence fence ) {
    // Reset scene command buffer
    cBufs_[imageIndex]->reset();

    // Initialize scene command buffer
    cBufs_[imageIndex]->prepare( currentFramebuffer,
                                 camera()->matrix<tire::VulkanTag>() );

    // Record draw commands for all scene objects in command buffer
    for ( size_t object = 0; object < buffersList_.size(); ++object ) {
        const auto color = bodyList_[object]->albedoColor().asVector3f();
        cBufs_[imageIndex]->bindBuffer(
            color, buffersList_[object]->buffer(),
            buffersList_[object]->verteciesCount() );
    }

    // Finalize command recording and submit command
    cBufs_[imageIndex]->submit( waitSemaphore, signalSemaphore, fence );
}

}  // namespace tire::vk
