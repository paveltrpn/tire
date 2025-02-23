
#include "rendervk.h"
#include "scene.h"

namespace tire::vk {

Scene::Scene( const std::filesystem::path &fname, const vk::Device *device,
              const vk::Pipeline *pipeline )
    : tire::Scene{ fname }
    , device_{ device }
    , pipeline_{ pipeline } {
    commandPool_ = std::make_unique<vk::CommandPool>( device_ );

    cBufs_.reserve( FRAMES_IN_FLIGHT_COUNT );
    for ( auto i = 0; i < cBufs_.capacity(); ++i ) {
        cBufs_.push_back( std::make_unique<vk::RenderFromBuffer>(
            device_, commandPool_.get() ) );
    }

    const auto nodeListSize = nodeList_.size();
    buffersList_.reserve( nodeListSize );

    for ( size_t i{}; i < nodeListSize; ++i ) {
        auto buf = std::make_shared<vk::VertexBuffer<float>>(
            device_, nodeList_[i]->verteciesArraySize() );

        buffersList_.push_back( std::move( buf ) );
    }
}

void Scene::submit() {
    for ( size_t i = 0; auto &buffer : buffersList_ ) {
        buffer->populate(
            reinterpret_cast<const void *>( nodeList_[i]->verteciesData() ) );
    }
}

void Scene::output( const VkFramebuffer currentFramebuffer, uint32_t imageIndex,
                    VkSemaphore waitSemaphore, VkSemaphore signalSemaphore,
                    VkFence fence ) {
    for ( size_t i = 0; const auto &buffer : buffersList_ ) {
        cBufs_[imageIndex]->setProperties( currentFramebuffer, pipeline_,
                                           buffer->buffer(), buffer->size() );
        cBufs_[imageIndex]->reset();
        cBufs_[imageIndex]->bind();
        cBufs_[imageIndex]->submit( waitSemaphore, signalSemaphore, fence );
    }
}

}  // namespace tire::vk
