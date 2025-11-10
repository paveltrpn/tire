#pragma once

#include <format>
#include <vector>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "context/context.h"

namespace tire {

struct Pipeline;

struct ContextQt final : Context {
    ContextQt( VkInstance instance, VkPhysicalDevice pDevice, VkDevice device,
               VkSurfaceKHR surface, VkRenderPass rp, uint32_t gqfi,
               uint32_t gqi );
    ~ContextQt() override;

    ContextQt( const ContextQt& other ) = delete;
    ContextQt( ContextQt&& other ) = delete;
    auto operator=( const ContextQt& other ) -> ContextQt& = delete;
    auto operator=( ContextQt&& other ) -> ContextQt& = delete;

    auto init() -> void override;

    auto queryDeviceInfo() -> void;
    auto querySurface() -> void;

    [[nodiscard]] auto currentExtent() const -> const VkExtent2D& override {
        return surfaceCapabilities_.currentExtent;
    };

    [[nodiscard]] auto getDeviceNameString() -> std::string {
        return std::string{
            static_cast<const char*>( pDeviceProperties_.deviceName ) };
    }

    [[nodiscard]] auto getDeviceDriverVersionString() -> std::string {
        return std::format(
            "{}.{}.{}", VK_VERSION_MAJOR( pDeviceProperties_.driverVersion ),
            VK_VERSION_MINOR( pDeviceProperties_.driverVersion ),
            VK_VERSION_PATCH( pDeviceProperties_.driverVersion ) );
    }

    [[nodiscard]] auto getApiVersionString() -> std::string {
        return std::format( "{}.{}.{}",
                            VK_VERSION_MAJOR( pDeviceProperties_.apiVersion ),
                            VK_VERSION_MINOR( pDeviceProperties_.apiVersion ),
                            VK_VERSION_PATCH( pDeviceProperties_.apiVersion ) );
    }

private:
    VkPhysicalDeviceProperties pDeviceProperties_{};
    VkPhysicalDeviceFeatures pDeviceFeatures_{};
    std::vector<VkExtensionProperties> pDeviceExtensions_{};
    std::vector<VkQueueFamilyProperties> queueFamilyProperties_{};

    VkSurfaceCapabilitiesKHR surfaceCapabilities_{};

    // Graphics queue resources.
    uint32_t graphicsQueueId_{};
};

}  // namespace tire