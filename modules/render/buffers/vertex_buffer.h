
#pragma once

#include <concepts>
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_core.h>

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_VERTEX_BUFFER_H{ true };

#include "../context.h"

namespace tire::vk {

template <std::floating_point T>
struct VertexBuffer final {
    using value_type = T;

    VertexBuffer( const vk::Context *context, size_t verteciesCount )
        : context_{ context }
        , verteciesCount_{ verteciesCount } {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        // Buffer size = sizeof(floating type) * vertecies count * three float coordinate
        bufferInfo.size = sizeof( value_type ) * verteciesCount_ * 3;
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if ( vkCreateBuffer( context_->device(), &bufferInfo, nullptr,
                             &buffer_ ) != VK_SUCCESS ) {
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
            throw std::runtime_error(
                "failed to allocate vertex buffer memory!" );
        }

        vkBindBufferMemory( context_->device(), buffer_, bufferMemory_, 0 );
    }

    void populate( const void *data ) {
        void *mapAddress;
        vkMapMemory( context_->device(), bufferMemory_, 0,
                     sizeof( value_type ) * verteciesCount_ * 3, 0,
                     &mapAddress );
        memcpy( mapAddress, data, sizeof( value_type ) * verteciesCount_ * 3 );
        vkUnmapMemory( context_->device(), bufferMemory_ );
    }

    VkBuffer buffer() { return buffer_; };
    VkDeviceMemory bufferMemory() { return bufferMemory_; };
    size_t verteciesCount() { return verteciesCount_; }

    void clean() {
        vkDestroyBuffer( context_->device(), buffer_, nullptr );
        vkFreeMemory( context_->device(), bufferMemory_, nullptr );
    }

private:
    const vk::Context *context_{};
    VkBuffer buffer_{ VK_NULL_HANDLE };
    VkDeviceMemory bufferMemory_{ VK_NULL_HANDLE };

    size_t verteciesCount_{};
};

}  // namespace tire::vk