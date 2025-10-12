
#pragma once

#include <cstring>
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_core.h>

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_VERTEX_BUFFER_H{ true };

#include "../context.h"

namespace tire::vk {

struct VertexBuffer final {
    using value_type = float;

    VertexBuffer( const vk::Context* context, size_t verteciesCount );

    void populate( const void* data );

    VkBuffer buffer() { return buffer_; };
    VkDeviceMemory bufferMemory() { return bufferMemory_; };
    size_t verteciesCount() { return verteciesCount_; }

    void clean();

private:
    const vk::Context* context_{};
    VkBuffer buffer_{ VK_NULL_HANDLE };
    VkDeviceMemory bufferMemory_{ VK_NULL_HANDLE };

    size_t verteciesCount_{};
};

}  // namespace tire::vk