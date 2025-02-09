
#pragma once

#include <concepts>
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_core.h>

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_VERTEX_BUFFER_H{ true };

#include "../device.h"

namespace tire::vk {

template <std::floating_point T>
struct VertexBuffer final {
    using value_type = T;

    VertexBuffer( const vk::Device *device, long long size )
        : device_{ device }
        , size_{ size } {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = sizeof( value_type ) * size_;
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if ( vkCreateBuffer( device_->handle(), &bufferInfo, nullptr,
                             &buffer_ ) != VK_SUCCESS ) {
            throw std::runtime_error( "failed to create vertex buffer!" );
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements( device_->handle(), buffer_,
                                       &memRequirements );

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = device_->memoryRequirements(
            memRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT );

        if ( vkAllocateMemory( device_->handle(), &allocInfo, nullptr,
                               &bufferMemory_ ) != VK_SUCCESS ) {
            throw std::runtime_error(
                "failed to allocate vertex buffer memory!" );
        }

        vkBindBufferMemory( device_->handle(), buffer_, bufferMemory_, 0 );
    }

    void populate( long long size, void *data ) {
        void *mapAddress;
        vkMapMemory( device_->handle(), bufferMemory_, 0, size, 0,
                     &mapAddress );
        memcpy( data, data, (size_t)size );
        vkUnmapMemory( device_->handle(), bufferMemory_ );
    }

    VkBuffer buffer() { return buffer_; };
    VkDeviceMemory bufferMemory() { return bufferMemory_; };
    long long size() { return size_; }

private:
    void clean() {
        vkDestroyBuffer( device_->handle(), buffer_, nullptr );
        vkFreeMemory( device_->handle(), bufferMemory_, nullptr );
    }

private:
    const vk::Device *device_{};
    VkBuffer buffer_{ VK_NULL_HANDLE };
    VkDeviceMemory bufferMemory_{ VK_NULL_HANDLE };

    long long size_{};
};

}  // namespace tire::vk