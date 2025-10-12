
#pragma once

#include <vulkan/vulkan.h>

#include "context.h"

namespace tire::vk {

struct CommandPool final {
    CommandPool( const vk::Context* context );

    CommandPool( const CommandPool& other ) = delete;
    CommandPool( CommandPool&& other ) = delete;
    CommandPool& operator=( const CommandPool& other ) = delete;
    CommandPool& operator=( CommandPool&& other ) = delete;

    ~CommandPool() {
        vkDestroyCommandPool( context_->device(), commandPool_, nullptr );
    }

    [[nodiscard]] VkCommandPool handle() const { return commandPool_; }

private:
    const vk::Context* context_{};

    // Vulkan etity handles
    VkCommandPool commandPool_{ VK_NULL_HANDLE };

    // Vulkan related structures
    VkCommandPoolCreateInfo poolInfo_{};
};

}  // namespace tire::vk
