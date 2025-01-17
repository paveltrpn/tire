 
#pragma once

#include <vulkan/vulkan.h>

#include "device.h"
#include "swapchain.h"

namespace tire::vk {

struct Present final {
    Present(const vk::Device *device, const vk::Swapchain *wapchain);

    Present( const Present &other ) = delete;
    Present( Present &&other ) = delete;
    Present &operator=( const Present &other ) = delete;
    Present &operator=( Present &&other ) = delete;

    ~Present() = default;

private:
    const vk::Device *device_{};
    const vk::Swapchain *swapchain_{};

    VkPresentInfoKHR presentInfo_{};
};

}
