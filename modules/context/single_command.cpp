
module;

#include <coroutine>
#include <exception>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

export module context:single_command;

import :context;

namespace tire {

export struct CommandRoutine {
    using value_type = VkCommandBuffer;

    struct promise_type;

    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        auto get_return_object() -> CommandRoutine {
            //
            return CommandRoutine{ handle_type::from_promise( *this ) };
        }

        auto initial_suspend() -> std::suspend_never {
            //
            return {};
        }

        auto final_suspend() noexcept -> std::suspend_never {
            //
            return {};
        }

        auto unhandled_exception() -> void {
            //
            exception_ = std::current_exception();
        }

        auto yield_value( VkCommandBuffer value ) -> std::suspend_always {
            value_ = value;
            return {};
        }

        auto return_void() -> void {
            //
        };

        value_type value_;
        std::exception_ptr exception_;
    };

    CommandRoutine( handle_type h )
        : h_( h ) {}

    ~CommandRoutine() {
        //
        post();
    }

    [[nodiscard]]
    auto buf() const -> VkCommandBuffer {
        return h_.promise().value_;
    }

private:
    void post() {
        h_.resume();
        if ( h_.promise().exception_ ) {
            std::rethrow_exception( h_.promise().exception_ );
        }
    }

private:
    handle_type h_;
};

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

auto Context::immidiateCommand() const -> CommandRoutine {
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

    co_yield commandBuffer;

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
