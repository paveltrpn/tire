
#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "instance.h"

namespace tire::vk {

struct Device final {
    Device( VkInstance instance, VkSurfaceKHR surface );

    Device( const Device &other ) = delete;
    Device( Device &&other ) = delete;
    Device &operator=( const Device &other ) = delete;
    Device &operator=( Device &&other ) = delete;

    ~Device();

    [[nodiscard]] VkDevice device() const;
    [[nodiscard]] uint32_t graphicsFamily() const { return graphicsFamily_; };
    [[nodiscard]] VkSurfaceFormatKHR surfaceFormat( size_t id ) const {
        return surfaceFormats_[id];
    };

    [[nodiscard]] VkPresentModeKHR presentMode( size_t id ) const {
        return presentModes_[id];
    };

    [[nodiscard]] VkSurfaceCapabilitiesKHR surfaceCapabilities() const {
        return surfaceCapabilities_;
    };

    [[nodiscard]] VkQueue graphicsQueue() const { return graphicsQueue_; };
    [[nodiscard]] VkQueue presentQueue() const { return presentQueue_; };

    void pickAndCreateDevice(
        const vk::Instance *instance,
        size_t id );  // TODO: make physical device pick smarter

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
    const VkInstance instance_{ VK_NULL_HANDLE };
    const VkSurfaceKHR surface_{ VK_NULL_HANDLE };

    VkDevice device_{ VK_NULL_HANDLE };
    std::vector<PhysicalDevice> physicalDevices_{};

    uint32_t graphicsFamily_{};
    uint32_t presentFamily_{};

    VkQueue graphicsQueue_{ VK_NULL_HANDLE };
    VkQueue presentQueue_{ VK_NULL_HANDLE };

    VkSurfaceCapabilitiesKHR surfaceCapabilities_{};
    std::vector<VkSurfaceFormatKHR> surfaceFormats_{};
    std::vector<VkPresentModeKHR> presentModes_{};
};

}  // namespace tire::vk
