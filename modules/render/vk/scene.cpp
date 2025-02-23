
#include <vector>

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
            device_, commandPool_.get() ) );
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
    }
}

void Scene::output( const VkFramebuffer currentFramebuffer, uint32_t imageIndex,
                    VkSemaphore waitSemaphore, VkSemaphore signalSemaphore,
                    VkFence fence ) {
    cBufs_[imageIndex]->reset();
    cBufs_[imageIndex]->begin( pipeline_, currentFramebuffer,
                               getCamera( 0 )->getMatrix() );

    std::vector<VkBuffer> buffers;
    std::vector<VkDeviceSize> offsets;
    uint32_t verteciesCount{};
    offsets.push_back( 0 );
    for ( size_t i = 0; const auto &buffer : buffersList_ ) {
        buffers.push_back( buffer->buffer() );
        offsets.push_back( buffer->size() );
        verteciesCount += buffer->size();
    }
    cBufs_[imageIndex]->draw( buffers, offsets, verteciesCount );
    cBufs_[imageIndex]->end( waitSemaphore, signalSemaphore, fence );
}

}  // namespace tire::vk
