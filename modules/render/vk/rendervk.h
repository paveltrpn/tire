
#pragma once

#include <print>
#include <string>
#include <vector>
#include <algorithm>
#include <expected>
#include <variant>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "config/config.h"
#include "../render.h"
#include "geometry/node.h"

namespace tire {

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
               VkDebugUtilsMessageTypeFlagsEXT messageType,
               const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
               void *pUserData ) {
    std::print( R"(validation layer debug:		{}
)",
                pCallbackData->pMessage );
    return VK_FALSE;
}

static VkResult vkCreateDebugUtilsMessenger(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger ) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkCreateDebugUtilsMessengerEXT" );
    if ( func != nullptr ) {
        return func( instance, pCreateInfo, pAllocator, pDebugMessenger );
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

struct RenderVK final : Render {
    RenderVK();
    ~RenderVK() override;

    void displayRenderInfo() override;
    void setSwapInterval( int interval ) override;
    void appendToRenderList( std::shared_ptr<tire::Node> node ) override;

    void initMainLoop() override;
    void preFrame() override;
    void frame() override;
    void postFrame() override;
    void swapBuffers() override;

private:
    void enumerateExtensionProperties();
    // pass std::nullopt to enable all available exensions
    std::vector<char *> makeExtensionsList(
        std::optional<std::vector<std::string>> list );
    void enumerateValidationLayers();
    // pass std::nullopt to enable all available validation layers.
    // may cause instance creation error, for example:
    // "Requested layer "VK_LAYER_VALVE_steam_overlay_32" was wrong bit-type!"
    std::vector<char *> makeValidationLayersList(
        std::optional<std::vector<std::string>> list );

    void createInstance();
    void initPhysicalDevices();
    void pickAndCreateDevice(
        size_t id );  // TODO: make physical device pick smarter
    void createSurface();

    void displayExtensionProperties();
    void displayValidationLayerProperties();
    void displayPhysicalDeviceProperties( size_t id );
    void displayPhysicalDeviceFeatures( size_t id );
    void displayPhysicalDeviceFamiliesProperties( size_t id );
    void displaySurfaceCapabilities();

private:
    std::string applicationName_;
    std::string engineName_;

    bool enableValidationLayers_{};

    // handles
    VkDebugUtilsMessengerEXT debugMessenger_{ VK_NULL_HANDLE };
    VkInstance instance_{ VK_NULL_HANDLE };
    VkSurfaceKHR surface_{ VK_NULL_HANDLE };
    VkDevice device_{ VK_NULL_HANDLE };
    VkQueue graphicsQueue_{ VK_NULL_HANDLE };
    VkQueue presentQueue_{ VK_NULL_HANDLE };

    // structs
    VkSurfaceCapabilitiesKHR surfaceCapabilities_{};

    // vulkan entities info lists
    std::vector<VkExtensionProperties> extensionProperties_{};
    std::vector<VkLayerProperties> layerProperties_{};
    std::vector<char *> validationLayersNames_{};
    std::vector<char *> extensionsNames_{};
    struct PhysicalDevice {
        VkPhysicalDevice device{ VK_NULL_HANDLE };
        VkPhysicalDeviceProperties properties{};
        VkPhysicalDeviceFeatures features{};
        std::vector<VkQueueFamilyProperties> queueFamilyProperties{};
    };
    std::vector<PhysicalDevice> physicalDevices_{};
};

}  // namespace tire
