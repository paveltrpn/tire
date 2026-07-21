
#include <vulkan/vk_enum_string_helper.h>

#include "vkinstance.h"
#include "device.h"

#include "allocator.h"
#include "log/log.h"

namespace tire {

VMAllocator::VMAllocator( const VKInstance *instance, const VKDevice *device ) {
    VmaVulkanFunctions vulkanFunctions{};
    vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo allocatorCreateInfo{};
    allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
    allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_3;
    allocatorCreateInfo.physicalDevice = device->physicalDevice();
    allocatorCreateInfo.device = device->get();
    allocatorCreateInfo.instance = instance->get();
    allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

    {
        const auto res = vmaCreateAllocator( &allocatorCreateInfo, &allocator_ );
        if ( res != VK_SUCCESS ) {
            log::fatal()( "failed to create allocator with code {}!", string_VkResult( res ) );
        } else {
            log::info()( "allocator created!" );
        }
    }
}

VMAllocator::~VMAllocator() {
    vmaDestroyAllocator( allocator_ );
}

[[nodiscard]] auto VMAllocator::get() const -> VmaAllocator {
    return allocator_;
}

}  // namespace tire