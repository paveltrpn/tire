
module;

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

export module context:single_command;

import :context;

namespace tire {

auto Context::beginSingleCommand() const -> VkCommandBuffer {
    const auto allocInfo = VkCommandBufferAllocateInfo{
      //
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .commandPool = commandPool_,
      .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
      .commandBufferCount = 1,
    };

    VkCommandBuffer commandBuffer{};
    vkAllocateCommandBuffers( device_, &allocInfo, &commandBuffer );

    const auto beginInfo = VkCommandBufferBeginInfo{
      //
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };

    vkBeginCommandBuffer( commandBuffer, &beginInfo );

    return commandBuffer;
}

auto Context::endSingleCommand( VkCommandBuffer commandBuffer ) const -> void {
    vkEndCommandBuffer( commandBuffer );

    const auto submitInfo = VkSubmitInfo{
      //
      .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .commandBufferCount = 1,
      .pCommandBuffers = &commandBuffer,
    };

    vkQueueSubmit( graphicsQueue_, 1, &submitInfo, VK_NULL_HANDLE );
    vkQueueWaitIdle( graphicsQueue_ );

    vkFreeCommandBuffers( device_, commandPool_, 1, &commandBuffer );
}

}  // namespace tire
