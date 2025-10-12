
#include "vertex_buffer.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_VERTEX_BUFFER_CPP{ true };

namespace tire::vk {

VertexBuffer::VertexBuffer( const vk::Context* context, size_t verteciesCount )
    : context_{ context }
    , verteciesCount_{ verteciesCount } {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    // Buffer size = sizeof(floating type) * vertecies count * three float coordinate
    bufferInfo.size = sizeof( value_type ) * verteciesCount_ * 3;
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if ( vkCreateBuffer( context_->device(), &bufferInfo, nullptr, &buffer_ ) !=
         VK_SUCCESS ) {
        throw std::runtime_error( "failed to create vertex buffer!" );
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements( context_->device(), buffer_,
                                   &memRequirements );

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = context_->memoryRequirements(
        memRequirements.memoryTypeBits,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT );

    if ( vkAllocateMemory( context_->device(), &allocInfo, nullptr,
                           &bufferMemory_ ) != VK_SUCCESS ) {
        throw std::runtime_error( "failed to allocate vertex buffer memory!" );
    }

    vkBindBufferMemory( context_->device(), buffer_, bufferMemory_, 0 );
}

void VertexBuffer::populate( const void* data ) {
    void* mapAddress;
    vkMapMemory( context_->device(), bufferMemory_, 0,
                 sizeof( value_type ) * verteciesCount_ * 3, 0, &mapAddress );
    std::memcpy( mapAddress, data, sizeof( value_type ) * verteciesCount_ * 3 );
    vkUnmapMemory( context_->device(), bufferMemory_ );
}

void VertexBuffer::clean() {
    vkDestroyBuffer( context_->device(), buffer_, nullptr );
    vkFreeMemory( context_->device(), bufferMemory_, nullptr );
}

}  // namespace tire::vk