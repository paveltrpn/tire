
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
#include "shader_storage.h"
#include "pipeline.h"
#include "commands.h"
#include "scene.h"

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

private:
    void scene( const std::filesystem::path &path ) override;
    void preLoop() override;
    void preFrame() override;
    void frame() override;
    void postFrame() override;
    void swapBuffers() override;
    void postLoop() override;

    // pass std::nullopt to enable all available exensions
    std::vector<char *> makeExtensionsList(
        std::optional<std::vector<std::string>> list );
    // pass std::nullopt to enable all available validation layers.
    std::vector<char *> makeValidationLayersList(
        std::optional<std::vector<std::string>> list );

    void createInstance();
    void initPhysicalDevices();
    void pickAndCreateDevice(
        size_t id );  // TODO: make physical device pick smarter
    void createSurface();
    void createSwapchain();
    void createImageViews();
    void createGraphicsPipeline();
    void createFramebuffers();
    void createSyncObjects();

    void displayExtensionProperties();
    void displayValidationLayerProperties();
    void displayPhysicalDeviceProperties( size_t id );
    void displayPhysicalDeviceFeatures( size_t id );
    void displayPhysicalDeviceFamiliesProperties( size_t id );
    void displaySurfaceCapabilities();

private:
    std::shared_ptr<vk::Scene> scene_;

    std::unique_ptr<vk::ShaderStorage> shaderStorage_{};
    std::unique_ptr<vk::PiplineSimple> pipelineSimple_{};
    std::unique_ptr<CommandPool> commandPool_{};
    std::vector<std::unique_ptr<CommandBuffer>> cBufs_{};

    // handles
    VkDebugUtilsMessengerEXT debugMessenger_{ VK_NULL_HANDLE };
    VkInstance instance_{ VK_NULL_HANDLE };
    VkSurfaceKHR surface_{ VK_NULL_HANDLE };
    VkDevice device_{ VK_NULL_HANDLE };
    VkQueue graphicsQueue_{ VK_NULL_HANDLE };
    VkQueue presentQueue_{ VK_NULL_HANDLE };
    VkSwapchainKHR swapChain_{ VK_NULL_HANDLE };
    std::vector<VkImage> swapChainImages_{};
    std::vector<VkImageView> swapChainImageViews_{};
    std::vector<VkFramebuffer> framebuffers_{};
    std::vector<VkSemaphore> imageAvailableSemaphores_{};
    std::vector<VkSemaphore> renderFinishedSemaphores_{};
    std::vector<VkFence> inFlightFences_{};

    // vulkan entities info
    std::vector<VkExtensionProperties> extensionProperties_{};
    std::vector<VkLayerProperties> layerProperties_{};
    std::vector<char *> validationLayersNames_{};
    std::vector<char *> extensionsNames_{};

    VkSurfaceCapabilitiesKHR surfaceCapabilities_{};
    std::vector<VkSurfaceFormatKHR> surfaceFormats_{};
    std::vector<VkPresentModeKHR> presentModes_{};

    struct PhysicalDevice {
        VkPhysicalDevice device{ VK_NULL_HANDLE };
        VkPhysicalDeviceProperties properties{};
        VkPhysicalDeviceFeatures features{};
        std::vector<VkExtensionProperties> extensions{};
        std::vector<VkQueueFamilyProperties> queueFamilyProperties{};
    };
    uint32_t graphicsFamily_{};
    uint32_t presentFamily_{};

    std::vector<PhysicalDevice> physicalDevices_{};

    VkFormat swapChainImageFormat_{};
    VkExtent2D swapChainExtent_{};

    uint32_t currentFrame_{ 0 };
};

}  // namespace tire
