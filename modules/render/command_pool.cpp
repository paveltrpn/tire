
#include <vulkan/vk_enum_string_helper.h>

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_COMMAND_POOL_CPP{ true };

#include "command_pool.h"

namespace tire::vk {

CommandPool::CommandPool( const vk::Context* context )
    : context_{ context } {
    poolInfo_.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo_.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo_.queueFamilyIndex = context_->graphicsFamily();

    if ( const auto err = vkCreateCommandPool( context_->device(), &poolInfo_,
                                               nullptr, &commandPool_ );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create command pool woth code {}!",
                         string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_COMMAND_POOL_CPP>(
            "vk::CommandPool === command pool created!" );
    }
}

CommandPool::~CommandPool() {
    vkDestroyCommandPool( context_->device(), commandPool_, nullptr );
}

}  // namespace tire::vk
