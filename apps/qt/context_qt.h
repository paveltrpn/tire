#pragma once

#include <format>
#include <vector>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "render/context.h"

#define CONFIG_APPLAICATION_NAME "application_name"
#define CONFIG_ENGINE_NAME "engine_name"
#define CONFIG_ENABLE_VALIDATION_LAYERS false
#define CONFIG_ENABLE_ADDITIONAL_VALIDATION_LAYERS false
#define CONFIG_ENABLE_API_DUMP_VALIDATION_LAYRES false
#define CONFIG_VERBOSE_VULKAN_OUTPUT false
#define CONFIG_ENABLE_RAYTRACING_APPLICATIONS false

namespace tire::vk {

struct Pipeline;

struct ContextQt final : Context {
    ContextQt( VkInstance instance, VkPhysicalDevice pDevice, VkDevice device,
               VkSurfaceKHR surface, VkRenderPass rp, uint32_t gqfi,
               uint32_t gqi );
    ~ContextQt() override;

    ContextQt( const ContextQt& other ) = delete;
    ContextQt( Context&& other ) = delete;
    ContextQt& operator=( const ContextQt& other ) = delete;
    ContextQt& operator=( ContextQt&& other ) = delete;

    auto queryDeviceInfo() -> void;
    auto querySurface() -> void;

    // [[nodiscard]] const VkSurfaceFormatKHR& surfaceFormat() const {
    // return surfaceFormat_;
    // };

    [[nodiscard]] auto memoryRequirements(
        uint32_t typeFilter, VkMemoryPropertyFlags properties ) const
        -> uint32_t override;

    [[nodiscard]] auto findSupportedFormat(
        const std::vector<VkFormat>& candidates, VkImageTiling tiling,
        VkFormatFeatureFlags features ) const -> VkFormat override;

    //[[nodiscard]] VkQueue presentQueue() const { return presentQueue_; };

    [[nodiscard]] auto currentExtent() const -> const VkExtent2D& override {
        return surfaceCapabilities_.currentExtent;
    };

    [[nodiscard]] auto getDeviceNameString() -> std::string {
        return pDeviceProperties_.deviceName;
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
    // Physical device
    VkPhysicalDevice pDevice_{ VK_NULL_HANDLE };
    VkPhysicalDeviceProperties pDeviceProperties_{};
    VkPhysicalDeviceFeatures pDeviceFeatures_{};
    std::vector<VkExtensionProperties> pDeviceExtensions_{};
    std::vector<VkQueueFamilyProperties> queueFamilyProperties_{};

    VkSurfaceCapabilitiesKHR surfaceCapabilities_{};

    // Graphics queue resources.
    uint32_t graphicsQueueId_{};
};

}  // namespace tire::vk