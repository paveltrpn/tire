
#pragma once

#include <array>
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

template <size_t count>
struct PresentSynchronization final {
    PresentSynchronization( const vk::Device *device )
        : device_{ device } {
        VkSemaphoreCreateInfo semaphoreInfo{
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0 };

        VkFenceCreateInfo fenceInfo{
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_FENCE_CREATE_SIGNALED_BIT };

        for ( size_t i = 0; i < count; i++ ) {
            if ( vkCreateSemaphore( device_->handle(), &semaphoreInfo, nullptr,
                                    &imageAvailableSemaphores_[i] ) !=
                     VK_SUCCESS ||
                 vkCreateSemaphore( device_->handle(), &semaphoreInfo, nullptr,
                                    &renderFinishedSemaphores_[i] ) !=
                     VK_SUCCESS ||
                 vkCreateFence( device_->handle(), &fenceInfo, nullptr,
                                &inFlightFences_[i] ) != VK_SUCCESS ) {
                throw std::runtime_error( "failed to create semaphores!" );
            }
        }
    }

    ~PresentSynchronization() {
        for ( auto i = 0; i < count; i++ ) {
            vkDestroySemaphore( device_->handle(), imageAvailableSemaphores_[i],
                                nullptr );
            vkDestroySemaphore( device_->handle(), renderFinishedSemaphores_[i],
                                nullptr );
            vkDestroyFence( device_->handle(), inFlightFences_[i], nullptr );
        }
    }

    [[nodiscard]] std::tuple<VkSemaphore, VkSemaphore, VkFence> get(
        size_t i ) {
        return { imageAvailableSemaphores_[i], renderFinishedSemaphores_[i],
                 inFlightFences_[i] };
    }

private:
    const vk::Device *device_{};

    std::array<VkSemaphore, count> imageAvailableSemaphores_{};
    std::array<VkSemaphore, count> renderFinishedSemaphores_{};
    std::array<VkFence, count> inFlightFences_{};
};

}  // namespace tire::vk
