
#pragma once

#include <vulkan/vulkan.h>

#include "device.h"
#include "swapchain.h"

namespace tire::vk {

struct Present final {
    Present( const vk::Device *device, const vk::Swapchain *swapchain );

    Present( const Present &other ) = delete;
    Present( Present &&other ) = delete;
    Present &operator=( const Present &other ) = delete;
    Present &operator=( Present &&other ) = delete;

    ~Present() = default;

    void present( const VkSemaphore semaphore, uint32_t *imageIndex );

private:
    const vk::Device *device_{};
    const vk::Swapchain *swapchain_{};
};

}  // namespace tire::vk
