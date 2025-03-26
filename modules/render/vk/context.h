
#pragma once

#include <vector>
#include <expected>

#include <vulkan/vulkan.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <vulkan/vulkan_core.h>

namespace tire::vk {

#define FRAMES_IN_FLIGHT_COUNT 2

struct Pipeline;

struct Context final {
    Context( Display* display, Window window );
    ~Context();

    Context( const Context& other ) = delete;
    Context( Context&& other ) = delete;
    Context& operator=( const Context& other ) = delete;
    Context& operator=( Context&& other ) = delete;

    // Init all context
    void init();

    [[nodiscard]] VkInstance instance() const { return instance_; };
    [[nodiscard]] VkSurfaceKHR surface() const { return surface_; };
    [[nodiscard]] VkDevice device() const { return device_; };
    [[nodiscard]] VkCommandPool commandPool() const { return commandPool_; };
    [[nodiscard]] VkSwapchainKHR swapchain() const { return swapchain_; };

    [[nodiscard]]
    const VkSurfaceFormatKHR& surfaceFormat() const {
        return surfaceFormat_;
    };

    [[nodiscard]]
    uint32_t memoryRequirements( uint32_t typeFilter,
                                 VkMemoryPropertyFlags properties ) const;
    [[nodiscard]]
    VkFormat findSupportedFormat( const std::vector<VkFormat>& candidates,
                                  VkImageTiling tiling,
                                  VkFormatFeatureFlags features ) const;

    void createFramebuffers( const Pipeline* pipeline );

    [[nodiscard]] std::tuple<VkSemaphore, VkSemaphore, VkFence>
    getPresentSynchronization( size_t i ) {
        return { imageAvailableSemaphores_[i], renderFinishedSemaphores_[i],
                 inFlightFences_[i] };
    }

    void present( const VkSemaphore semaphore, uint32_t* imageIndex );

    [[nodiscard]] VkFramebuffer framebuffer( size_t id ) const {
        return framebuffers_[id];
    };

    [[nodiscard]]
    const VkQueue& graphicsQueue() const {
        return graphicsQueue_;
    }

    [[nodiscard]] VkQueue presentQueue() const { return presentQueue_; };

    [[nodiscard]] uint32_t graphicsFamily() const {
        return graphicsFamilyQueueId_;
    };

    [[nodiscard]]
    const VkExtent2D& currentExtent() const {
        return currentExtent_;
    };

private:
    void makeInstance();
    void makeSurface();
    void collectPhysicalDevices();
    void makeDevice();
    void makeCommandPool();
    void makeSwapchain();
    void makeImageViews();
    void makePresentSynchronization();

private:
    struct PhysicalDevice {
        VkPhysicalDevice device{ VK_NULL_HANDLE };
        VkPhysicalDeviceProperties properties{};
        VkPhysicalDeviceFeatures features{};
        std::vector<VkExtensionProperties> extensions{};
        std::vector<VkQueueFamilyProperties> queueFamilyProperties{};
    };

private:
    // Outer world connection
    const Display* display_;
    const Window window_;

    // Instance
    VkInstance instance_{ VK_NULL_HANDLE };
    VkDebugUtilsMessengerEXT debugMessenger_{ VK_NULL_HANDLE };
    std::vector<const char*> desiredValidationLayerList_{};
    std::vector<VkExtensionProperties> extensionProperties_{};
    std::vector<VkLayerProperties> layerProperties_{};

    // Surface
    VkSurfaceKHR surface_{ VK_NULL_HANDLE };

    // Physical and logical devices
    VkDevice device_{ VK_NULL_HANDLE };
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
    VkExtent2D currentExtent_{};

    // Command pool
    VkCommandPool commandPool_{ VK_NULL_HANDLE };

    // Swapchain
    VkSwapchainKHR swapchain_{ VK_NULL_HANDLE };
    uint32_t swapchainImageCount_{};
    std::vector<VkImage> swapChainImages_{};
    std::vector<VkImageView> swapChainImageViews_{};
    std::vector<VkFramebuffer> framebuffers_{};
    VkImage depthImage_;
    VkDeviceMemory depthImageMemory_;
    VkImageView depthImageView_;

    // Present synchronization
    std::array<VkSemaphore, FRAMES_IN_FLIGHT_COUNT> imageAvailableSemaphores_{};
    std::array<VkSemaphore, FRAMES_IN_FLIGHT_COUNT> renderFinishedSemaphores_{};
    std::array<VkFence, FRAMES_IN_FLIGHT_COUNT> inFlightFences_{};
};

}  // namespace tire::vk