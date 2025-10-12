
#pragma once

#include <vector>
#include <expected>

#include <vulkan/vulkan.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <vulkan/vulkan_core.h>

namespace tire::vk {

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

    void makeFrames( const Pipeline* pipeline );

    [[nodiscard]] std::tuple<VkSemaphore, VkSemaphore, VkFence> getFrameSyncSet(
        size_t id ) {
        return { frames_[id].imageAvailableSemaphore_,
                 frames_[id].renderFinishedSemaphore_,
                 frames_[id].inFlightFence_ };
    }

    [[nodiscard]] uint32_t framesCount() const { return framesCount_; };

    void present( const VkSemaphore semaphore, uint32_t* imageIndex );

    [[nodiscard]] VkFramebuffer framebuffer( size_t id ) const {
        return frames_[id].framebuffer_;
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
    void makeXlibSurface();
    void collectPhysicalDevices();
    void makeDevice();
    void makeCommandPool();
    void makeSwapchain();
    auto initPrimaryCommandBuffer() -> void;
    auto initSecondaryCommandBuffer() -> void;

private:
    struct PhysicalDevice final {
        VkPhysicalDevice device{ VK_NULL_HANDLE };
        VkPhysicalDeviceProperties properties{};
        VkPhysicalDeviceFeatures features{};
        std::vector<VkExtensionProperties> extensions{};
        std::vector<VkQueueFamilyProperties> queueFamilyProperties{};
    };

    struct Frame final {
        VkImage image_{};
        VkImageView view_{};
        VkFramebuffer framebuffer_{};
        VkSemaphore imageAvailableSemaphore_{};
        VkSemaphore renderFinishedSemaphore_{};
        VkFence inFlightFence_{};
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
    VkCommandBuffer cbPrimary_{ VK_NULL_HANDLE };
    VkCommandBuffer cbSecondary_{ VK_NULL_HANDLE };
    // Background color value
    std::array<VkClearValue, 2> clearValues_{};

    // Swapchain
    VkSwapchainKHR swapchain_{ VK_NULL_HANDLE };
    uint32_t swapchainImageCount_{};
    uint32_t framesCount_{};
    std::vector<Frame> frames_{};
    VkImage depthImage_;
    VkDeviceMemory depthImageMemory_;
    VkImageView depthImageView_;
};

}  // namespace tire::vk