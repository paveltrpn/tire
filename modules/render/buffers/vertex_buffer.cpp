
module;

#include <cstring>
#include <format>

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_core.h>

#include "vma/vk_mem_alloc.h"

export module render:vertex_buffer;

import context;

namespace tire {

struct VertexBuffer final {
    using value_type = float;

    VertexBuffer() = delete;
    VertexBuffer( const VertexBuffer &other ) = delete;
    VertexBuffer( VertexBuffer &&other ) = delete;

    VertexBuffer( const Context *context, size_t size )
        : context_{ context }
        , size_{ size } {
        //
        initStagingBuffer( size );
        initDeviceBuffer( size );
    }

    auto operator=( const VertexBuffer &other ) -> VertexBuffer & = delete;
    auto operator=( VertexBuffer &&other ) -> VertexBuffer & = delete;

    ~VertexBuffer() {
        //
        clean();
    };

    auto deviceBuffer() -> VkBuffer {
        //
        return deviceBuffer_;
    }

    auto stagingBuffer() -> VkBuffer {
        //
        return stagingBuffer_;
    }

    auto populate( const void *data ) -> void {
        void *mappedPtr{};
        vmaMapMemory( context_->allocator(), stagingAllocation_, &mappedPtr );
        memcpy( mappedPtr, data, size_ );
        vmaUnmapMemory( context_->allocator(), stagingAllocation_ );
    }

    [[nodiscard]]
    auto size() -> size_t {
        //
        return size_;
    }

    auto clean() -> void {
        //
        vmaDestroyBuffer( context_->allocator(), deviceBuffer_, deviceAllocation_ );
        vmaDestroyBuffer( context_->allocator(), stagingBuffer_, stagingAllocation_ );
    }

private:
    auto initStagingBuffer( size_t size ) -> void {
        VkBufferCreateInfo stagingBufferInfo{};
        stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        stagingBufferInfo.pNext = nullptr;

        stagingBufferInfo.size = size;
        stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

        VmaAllocationCreateInfo vmaallocInfo{};
        vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

        vmaCreateBuffer(
          context_->allocator(), &stagingBufferInfo, &vmaallocInfo, &stagingBuffer_, &stagingAllocation_, nullptr );
    }

    auto initDeviceBuffer( size_t size ) -> void {
        VkBufferCreateInfo bufCreateInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
        bufCreateInfo.size = size_;
        bufCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

        VmaAllocationCreateInfo allocCreateInfo = {};
        allocCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
        allocCreateInfo.flags =
          VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;

        vmaCreateBuffer(
          context_->allocator(), &bufCreateInfo, &allocCreateInfo, &deviceBuffer_, &deviceAllocation_, nullptr );
    }

private:
    const Context *context_{};
    size_t size_{};

    VkBuffer deviceBuffer_{ VK_NULL_HANDLE };
    VmaAllocation deviceAllocation_{ VK_NULL_HANDLE };

    VkBuffer stagingBuffer_{ VK_NULL_HANDLE };
    VmaAllocation stagingAllocation_{ VK_NULL_HANDLE };
};

}  // namespace tire
