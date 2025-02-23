
#include <vector>

#include "algebra/vector4.h"
#include "render/vk/commands/scene_render_command.h"
#include "rendervk.h"
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
            device_, pipeline_, commandPool_.get(), nodeList_.size() ) );
    }

    // Create vulkan "vertex buffers"
    const auto nodeListSize = nodeList_.size();
    buffersList_.reserve( nodeListSize );
    for ( size_t i{}; i < nodeListSize; ++i ) {
        auto buf = std::make_shared<vk::VertexBuffer<float>>(
            device_, nodeList_[i]->verteciesArraySize() );

        buffersList_.push_back( std::move( buf ) );
    }
}

void Scene::submit() {
    // Update data in vulkan "vertex" buffers, i.e. copy from CPU memory
    for ( size_t i = 0; auto &buffer : buffersList_ ) {
        buffer->populate(
            reinterpret_cast<const void *>( nodeList_[i]->verteciesData() ) );
        ++i;
    }
}

void Scene::output( const VkFramebuffer currentFramebuffer, uint32_t imageIndex,
                    VkSemaphore waitSemaphore, VkSemaphore signalSemaphore,
                    VkFence fence ) {
#define OBJECT 5
    for ( size_t i = 0; i < buffersList_.size(); ++i ) {
        algebra::vector3f color = { nodeList_[OBJECT]->color().r(),
                                    nodeList_[OBJECT]->color().g(),
                                    nodeList_[OBJECT]->color().b() };
        cBufs_[imageIndex]->reset( i );
        cBufs_[imageIndex]->prepare(
            i, currentFramebuffer, getCamera( 0 )->getMatrix(), color,
            buffersList_[OBJECT]->buffer(), buffersList_[OBJECT]->size() );
    }
    cBufs_[imageIndex]->submit( waitSemaphore, signalSemaphore, fence );
}

}  // namespace tire::vk
