#pragma once

#include <cstddef>
#include <cstring>

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_core.h>

#include "vma/vk_mem_alloc.h"

namespace tire {

struct VertexBuffer final {
    using value_type = float;

    explicit VertexBuffer( size_t size );

    VertexBuffer( const VertexBuffer& other ) = delete;
    VertexBuffer( VertexBuffer&& other ) noexcept;

    auto operator=( const VertexBuffer& other ) -> VertexBuffer& = delete;

    auto operator=( VertexBuffer&& other ) noexcept -> VertexBuffer&;

    ~VertexBuffer();

    [[nodiscard]]
    auto deviceBuffer() const -> VkBuffer;

    [[nodiscard]]
    auto stagingBuffer() const -> VkBuffer;

    auto memcpy( const void* data, size_t size, size_t offset = 0 ) const -> void;

    [[nodiscard]]
    auto size() const -> size_t;

    auto clean() -> void;

private:
    auto initStagingBuffer( size_t size ) -> void;
    auto initDeviceBuffer( size_t size ) -> void;

private:
    size_t _size{};

    VkBuffer _deviceBuffer{ VK_NULL_HANDLE };
    VmaAllocation _deviceAllocation{ VK_NULL_HANDLE };

    VkBuffer _stagingBuffer{ VK_NULL_HANDLE };
    VmaAllocation _stagingAllocation{ VK_NULL_HANDLE };
};

}  // namespace tire
