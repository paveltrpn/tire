
#include <vector>

#include "algebra/vector4.h"
#include "render/vk/commands/scene_render_command.h"
#include "rendervk.h"
#include "vulkan/vulkan_core.h"
#include "scene.h"

namespace tire::vk {

Scene::Scene( const std::filesystem::path &fname, const vk::Context *context,
              const vk::Pipeline *pipeline )
    : tire::Scene{ fname }
    , context_{ context }
    , pipeline_{ pipeline } {
    // Create scene draw command for each frame
    cBufs_.reserve( context_->framesCount() );
    for ( auto i = 0; i < cBufs_.capacity(); ++i ) {
        cBufs_.push_back(
            std::make_unique<vk::SceneRenderCommand>( context_, pipeline_ ) );
    }

    // Create vulkan "vertex buffers"
    const auto nodeListSize = bodyList_.size();
    vertBuffersList_.reserve( nodeListSize );
    for ( size_t i{}; i < nodeListSize; ++i ) {
        auto buf = std::make_shared<vk::VertexBuffer<float>>(
            context_, bodyList_[i]->verteciesCount() );
        vertBuffersList_.push_back( std::move( buf ) );
    }

    // Create vulkan "normal buffers"
    nrmlBuffersList_.reserve( nodeListSize );
    for ( size_t i{}; i < nodeListSize; ++i ) {
        auto buf = std::make_shared<vk::VertexBuffer<float>>(
            context_, bodyList_[i]->verteciesCount() );
        nrmlBuffersList_.push_back( std::move( buf ) );
    }
}

void Scene::submit() {
    // Update data in vulkan "vertex" buffers, i.e. copy from CPU memory
    for ( size_t i = 0; auto &buffer : vertBuffersList_ ) {
        buffer->populate(
            reinterpret_cast<const void *>( bodyList_[i]->verteciesData() ) );
        ++i;
    }

    // Update data in vulkan "normal" buffers
    for ( size_t i = 0; auto &buffer : nrmlBuffersList_ ) {
        buffer->populate(
            reinterpret_cast<const void *>( bodyList_[i]->normalsData() ) );
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
                                 camera().matrix<tire::VulkanTag>() );

    // Record draw commands for all scene objects in command buffer
    for ( size_t object = 0; object < vertBuffersList_.size(); ++object ) {
        cBufs_[imageIndex]->bindBuffer(
            bodyList_[object]->albedoColor(),
            vertBuffersList_[object]->buffer(),
            nrmlBuffersList_[object]->buffer(),
            vertBuffersList_[object]->verteciesCount() );
    }

    // Finalize command recording and submit command
    cBufs_[imageIndex]->submit( waitSemaphore, signalSemaphore, fence );
}

}  // namespace tire::vk
