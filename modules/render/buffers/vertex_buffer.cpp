
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
        VkBufferCreateInfo bufCreateInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
        bufCreateInfo.size = size_;
        bufCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

        VmaAllocationCreateInfo allocCreateInfo = {};
        allocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
        allocCreateInfo.flags =
          VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;

        vmaCreateBuffer( context->allocator(), &bufCreateInfo, &allocCreateInfo, &buffer_, &allocation_, &allocInfo_ );
    }

    auto operator=( const VertexBuffer &other ) -> VertexBuffer & = delete;
    auto operator=( VertexBuffer &&other ) -> VertexBuffer & = delete;

    ~VertexBuffer() {
        //
        clean();
    };

    auto populate( const void *data ) -> void {
        //
        memcpy( allocInfo_.pMappedData, data, size_ );
    }

    [[nodiscard]]
    auto buffer() const -> VkBuffer {
        //
        return buffer_;
    };

    [[nodiscard]]
    auto size() -> size_t {
        //
        return size_;
    }

    auto clean() -> void {
        //
        vmaDestroyBuffer( context_->allocator(), buffer_, allocation_ );
    }

private:
    const Context *context_{};
    size_t size_{};

    VmaAllocationInfo allocInfo_{};
    VkBuffer buffer_{ VK_NULL_HANDLE };
    VmaAllocation allocation_{ VK_NULL_HANDLE };
};

}  // namespace tire
