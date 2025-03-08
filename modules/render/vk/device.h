
#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "instance.h"
#include "surface.h"

namespace tire::vk {

struct Device final {
    Device( const vk::Instance *instance, const vk::Surface *surface );

    Device( const Device &other ) = delete;
    Device( Device &&other ) = delete;
    Device &operator=( const Device &other ) = delete;
    Device &operator=( Device &&other ) = delete;

    ~Device();

    [[nodiscard]] VkDevice handle() const;
    [[nodiscard]] uint32_t graphicsFamily() const {
        return graphicsFamilyQueueId_;
    };
    [[nodiscard]] uint32_t presentFamily() const {
        return presentSupportQueueId_;
    };
    [[nodiscard]] VkSurfaceFormatKHR surfaceFormat() const {
        return surfaceFormat_;
    };

    [[nodiscard]] VkPresentModeKHR presentMode() const { return presentMode_; };

    [[nodiscard]] VkSurfaceCapabilitiesKHR surfaceCapabilities() const {
        return surfaceCapabilities_;
    };

    [[nodiscard]] VkQueue graphicsQueue() const { return graphicsQueue_; };
    [[nodiscard]] VkQueue presentQueue() const { return presentQueue_; };

    [[nodiscard]] VkExtent2D extent() const { return currentExtent_; };

    [[nodiscard]] uint32_t memoryRequirements(
        uint32_t typeFilter, VkMemoryPropertyFlags properties ) const;
    [[nodiscard]] VkFormat findSupportedFormat(
        const std::vector<VkFormat> &candidates, VkImageTiling tiling,
        VkFormatFeatureFlags features ) const;

    void pickAndCreateDevice();

    void displayRenderInfo();
    void displayPhysicalDeviceProperties( size_t id );
    void displayPhysicalDeviceFeatures( size_t id );
    void displayPhysicalDeviceFamiliesProperties( size_t id );
    void displaySurfaceCapabilities();

private:
    struct PhysicalDevice {
        VkPhysicalDevice device{ VK_NULL_HANDLE };
        VkPhysicalDeviceProperties properties{};
        VkPhysicalDeviceFeatures features{};
        std::vector<VkExtensionProperties> extensions{};
        std::vector<VkQueueFamilyProperties> queueFamilyProperties{};
    };

private:
    const vk::Instance *instance_{};
    const vk::Surface *surface_{};

    // All app logical device handle
    VkDevice device_{ VK_NULL_HANDLE };
    // System wide available physical devices
    std::vector<PhysicalDevice> physicalDevices_{};

    int pickedPhysicalDeviceId_{ -1 };

    uint32_t graphicsFamilyQueueId_{ UINT32_MAX };
    uint32_t presentSupportQueueId_{ UINT32_MAX };

    VkQueue graphicsQueue_{ VK_NULL_HANDLE };
    VkQueue presentQueue_{ VK_NULL_HANDLE };

    VkSurfaceCapabilitiesKHR surfaceCapabilities_{};
    std::vector<VkSurfaceFormatKHR> surfaceFormats_{};
    std::vector<VkPresentModeKHR> presentModes_{};

    VkSurfaceFormatKHR surfaceFormat_{};
    VkPresentModeKHR presentMode_{};

    // Current application window extent
    VkExtent2D currentExtent_{};
};

}  // namespace tire::vk
