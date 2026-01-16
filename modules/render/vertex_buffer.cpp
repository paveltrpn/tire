
module;

#include <cstddef>
#include <utility>
#include <cstring>
#include <format>

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_core.h>

#include "vma/vk_mem_alloc.h"

export module render:vertex_buffer;

import log;
import context;

namespace tire {

struct VertexBuffer final {
    using value_type = float;

    VertexBuffer() = delete;
    VertexBuffer( const VertexBuffer &other ) = delete;

    VertexBuffer( VertexBuffer &&other ) noexcept {
        //
        context_ = std::exchange( other.context_, nullptr );
        size_ = std::exchange( other.size_, 0 );

        deviceBuffer_ = std::exchange( other.deviceBuffer_, VK_NULL_HANDLE );
        deviceAllocation_ = std::exchange( other.deviceAllocation_, VK_NULL_HANDLE );

        stagingBuffer_ = std::exchange( other.stagingBuffer_, VK_NULL_HANDLE );
        stagingAllocation_ = std::exchange( other.stagingAllocation_, VK_NULL_HANDLE );
    }

    VertexBuffer( const Context *context, size_t size )
        : context_{ context }
        , size_{ size } {
        //
        initStagingBuffer( size );
        initDeviceBuffer( size );
    }

    auto operator=( const VertexBuffer &other ) -> VertexBuffer & = delete;

    auto operator=( VertexBuffer &&other ) noexcept -> VertexBuffer & {
        //
        context_ = std::exchange( other.context_, nullptr );
        size_ = std::exchange( other.size_, 0 );

        deviceBuffer_ = std::exchange( other.deviceBuffer_, VK_NULL_HANDLE );
        deviceAllocation_ = std::exchange( other.deviceAllocation_, VK_NULL_HANDLE );

        stagingBuffer_ = std::exchange( other.stagingBuffer_, VK_NULL_HANDLE );
        stagingAllocation_ = std::exchange( other.stagingAllocation_, VK_NULL_HANDLE );

        return *this;
    }

    ~VertexBuffer() {
        //
        clean();
    };

    [[nodiscard]]
    auto deviceBuffer() const -> VkBuffer {
        //
        return deviceBuffer_;
    }

    [[nodiscard]]
    auto stagingBuffer() const -> VkBuffer {
        //
        return stagingBuffer_;
    }

    auto memcpy( const void *data, size_t size, size_t offset = 0 ) const -> void {
        if ( size > size_ ) {
            log::warning()( "target memory chunk larger than allocated!" );
        }

        void *mappedPtr{};
        vmaMapMemory( context_->allocator(), stagingAllocation_, &mappedPtr );

        char *offsettedPtr = static_cast<char *>( mappedPtr ) + offset;

        std::memcpy( offsettedPtr, data, size );

        vmaUnmapMemory( context_->allocator(), stagingAllocation_ );
    }

    [[nodiscard]]
    auto size() const -> size_t {
        //
        return size_;
    }

    auto clean() -> void {
        //
        // vmaDestroyBuffer( context_->allocator(), deviceBuffer_, deviceAllocation_ );
        // vmaDestroyBuffer( context_->allocator(), stagingBuffer_, stagingAllocation_ );
    }

private:
    auto initStagingBuffer( size_t size ) -> void {
        const auto stagingBufferInfo = VkBufferCreateInfo{
          //
          .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
          .pNext = nullptr,
          .size = size,
          .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,

        };

        const auto vmaallocInfo = VmaAllocationCreateInfo{
          //
          .usage = VMA_MEMORY_USAGE_CPU_ONLY,
        };

        vmaCreateBuffer(
          context_->allocator(), &stagingBufferInfo, &vmaallocInfo, &stagingBuffer_, &stagingAllocation_, nullptr );
    }

    auto initDeviceBuffer( size_t size ) -> void {
        const auto bufCreateInfo = VkBufferCreateInfo{
          //
          .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
          .size = size_,
          .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        };

        const auto allocCreateInfo = VmaAllocationCreateInfo{
          //
          .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
          .usage = VMA_MEMORY_USAGE_GPU_ONLY,

        };

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
