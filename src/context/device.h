
#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace tire {

struct VKDvice final {
    VKDvice( const VKDvice &other ) = delete;
    VKDvice( VKDvice &&other ) = delete;

    auto operator=( const VKDvice &other ) -> VKDvice & = delete;
    auto operator=( VKDvice &&other ) -> VKDvice & = delete;

    ~VKDvice() = default;

    [[nodiscard]] auto get() -> VkDevice;

private:
    struct PhysicalDevice final {
        VkPhysicalDevice device{ VK_NULL_HANDLE };
        VkPhysicalDeviceProperties properties{};
        VkPhysicalDeviceFeatures features{};
        std::vector<VkExtensionProperties> extensions{};
        std::vector<VkQueueFamilyProperties> queueFamilyProperties{};
    };

private:

    std::vector<PhysicalDevice> physicalDevices_{};
    int pickedPhysicalDeviceId_{ -1 };
    uint32_t presentSupportQueueId_{ UINT32_MAX };
    VkQueue presentQueue_{ VK_NULL_HANDLE };
    VkSurfaceCapabilitiesKHR surfaceCapabilities_{};
    std::vector<VkSurfaceFormatKHR> surfaceFormats_{};
    std::vector<VkPresentModeKHR> presentModes_{};
    VkPresentModeKHR presentMode_{};
    VkPhysicalDevice physDevice_{};
    // The logical device itself.
    VkDevice device_{ VK_NULL_HANDLE };
};

}