
#include <cstddef>
#include <utility>
#include <cstring>
#include <format>

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_core.h>

#include "vma/vk_mem_alloc.h"

#include "vertex_buffer.h"
#include "context/context.h"
#include "log/log.h"

namespace tire {

VertexBuffer::VertexBuffer( VertexBuffer&& other ) noexcept {
    _size = std::exchange( other._size, 0 );

    _deviceBuffer = std::exchange( other._deviceBuffer, VK_NULL_HANDLE );
    _deviceAllocation = std::exchange( other._deviceAllocation, VK_NULL_HANDLE );

    _stagingBuffer = std::exchange( other._stagingBuffer, VK_NULL_HANDLE );
    _stagingAllocation = std::exchange( other._stagingAllocation, VK_NULL_HANDLE );
}

VertexBuffer::VertexBuffer( size_t size )
    : _size{ size } {
    initStagingBuffer( size );
    initDeviceBuffer( size );
}

auto VertexBuffer::operator=( VertexBuffer&& other ) noexcept -> VertexBuffer& {
    _size = std::exchange( other._size, 0 );

    _deviceBuffer = std::exchange( other._deviceBuffer, VK_NULL_HANDLE );
    _deviceAllocation = std::exchange( other._deviceAllocation, VK_NULL_HANDLE );

    _stagingBuffer = std::exchange( other._stagingBuffer, VK_NULL_HANDLE );
    _stagingAllocation = std::exchange( other._stagingAllocation, VK_NULL_HANDLE );

    return *this;
}

VertexBuffer::~VertexBuffer() {
    //
    clean();
};

auto VertexBuffer::deviceBuffer() const -> VkBuffer {
    //
    return _deviceBuffer;
}

auto VertexBuffer::stagingBuffer() const -> VkBuffer {
    //
    return _stagingBuffer;
}

auto VertexBuffer::memcpy( const void* data, size_t size, size_t offset ) const -> void {
    if ( size > _size ) {
        log::warning()( "target memory chunk larger than allocated!" );
    }

    void* mappedPtr{};
    vmaMapMemory( Context::instance().allocator(), _stagingAllocation, &mappedPtr );

    char* offsettedPtr = static_cast<char*>( mappedPtr ) + offset;

    std::memcpy( offsettedPtr, data, size );

    vmaUnmapMemory( Context::instance().allocator(), _stagingAllocation );
}

auto VertexBuffer::size() const -> size_t {
    //
    return _size;
}

auto VertexBuffer::clean() -> void {
    vmaDestroyBuffer( Context::instance().allocator(), _deviceBuffer, _deviceAllocation );
    vmaDestroyBuffer( Context::instance().allocator(), _stagingBuffer, _stagingAllocation );
}

auto VertexBuffer::initStagingBuffer( size_t size ) -> void {
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

    vmaCreateBuffer( Context::instance().allocator(), &stagingBufferInfo, &vmaallocInfo, &_stagingBuffer,
                     &_stagingAllocation, nullptr );
}

auto VertexBuffer::initDeviceBuffer( size_t size ) -> void {
    const auto bufCreateInfo = VkBufferCreateInfo{
        //
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = _size,
        .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    };

    const auto allocCreateInfo = VmaAllocationCreateInfo{
        //
        .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .usage = VMA_MEMORY_USAGE_GPU_ONLY,

    };

    vmaCreateBuffer( Context::instance().allocator(), &bufCreateInfo, &allocCreateInfo, &_deviceBuffer,
                     &_deviceAllocation, nullptr );
}

}  // namespace tire
