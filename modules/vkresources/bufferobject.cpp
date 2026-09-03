module;

#include <cstddef>
#include <cstring>
#include <utility>

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_core.h>

#include "vma/vk_mem_alloc.h"

#include "context/context.h"
#include "log/log.h"

export module vkresources : bufferobject;

namespace tire {

export struct BufferObject final {
    using value_type = float;

    explicit BufferObject( size_t size )
        : _size{ size } {
        auto* allocator = Context::instance().allocator();

        // Create staging buffer.
        {
            const auto bufferCreateInfo = VkBufferCreateInfo{
                .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                .pNext = nullptr,
                .size = size,
                .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,

            };

            const auto vmaallocInfo = VmaAllocationCreateInfo{
                .usage = VMA_MEMORY_USAGE_CPU_ONLY,
            };

            vmaCreateBuffer( allocator, &bufferCreateInfo, &vmaallocInfo, &_stagingBuffer, &_stagingAllocation,
                             nullptr );
        }

        // Create device buffer.
        {
            const auto bufferCreateInfo = VkBufferCreateInfo{
                .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                .size = _size,
                .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            };

            const auto vmaallocInfo = VmaAllocationCreateInfo{
                .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
                .usage = VMA_MEMORY_USAGE_GPU_ONLY,

            };

            vmaCreateBuffer( allocator, &bufferCreateInfo, &vmaallocInfo, &_deviceBuffer, &_deviceAllocation, nullptr );
        }
    }

    BufferObject( const BufferObject& other ) = delete;

    BufferObject( BufferObject&& other ) noexcept {
        _size = std::exchange( other._size, 0 );

        _deviceBuffer = std::exchange( other._deviceBuffer, VK_NULL_HANDLE );
        _deviceAllocation = std::exchange( other._deviceAllocation, VK_NULL_HANDLE );

        _stagingBuffer = std::exchange( other._stagingBuffer, VK_NULL_HANDLE );
        _stagingAllocation = std::exchange( other._stagingAllocation, VK_NULL_HANDLE );
    }

    auto operator=( const BufferObject& other ) -> BufferObject& = delete;

    auto operator=( BufferObject&& other ) noexcept -> BufferObject& {
        _size = std::exchange( other._size, 0 );

        _deviceBuffer = std::exchange( other._deviceBuffer, VK_NULL_HANDLE );
        _deviceAllocation = std::exchange( other._deviceAllocation, VK_NULL_HANDLE );

        _stagingBuffer = std::exchange( other._stagingBuffer, VK_NULL_HANDLE );
        _stagingAllocation = std::exchange( other._stagingAllocation, VK_NULL_HANDLE );

        return *this;
    }

    ~BufferObject() {
        //
        clean();
    };

    [[nodiscard]]
    auto deviceBuffer() const -> VkBuffer {
        //
        return _deviceBuffer;
    }

    [[nodiscard]]
    auto stagingBuffer() const -> VkBuffer {
        //
        return _stagingBuffer;
    }

    auto memcpy( const void* data, size_t size, size_t offset ) const -> void {
        if ( size > _size ) {
            log::warning()( "target memory chunk larger than allocated!" );
        }

        auto* allocator = Context::instance().allocator();

        void* mappedPtr{};
        vmaMapMemory( allocator, _stagingAllocation, &mappedPtr );

        char* offsettedPtr = static_cast<char*>( mappedPtr ) + offset;

        std::memcpy( offsettedPtr, data, size );

        vmaUnmapMemory( allocator, _stagingAllocation );
    }

    [[nodiscard]]
    auto size() const -> size_t {
        //
        return _size;
    }

    auto clean() -> void {
        auto* allocator = Context::instance().allocator();

        vmaDestroyBuffer( allocator, _deviceBuffer, _deviceAllocation );
        vmaDestroyBuffer( allocator, _stagingBuffer, _stagingAllocation );
    }

private:
    size_t _size{};

    VkBuffer _deviceBuffer{ VK_NULL_HANDLE };
    VmaAllocation _deviceAllocation{ VK_NULL_HANDLE };

    VkBuffer _stagingBuffer{ VK_NULL_HANDLE };
    VmaAllocation _stagingAllocation{ VK_NULL_HANDLE };
};

}  // namespace tire
