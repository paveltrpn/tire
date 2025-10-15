
#pragma once

#include <vector>
#include <array>

#include <vulkan/vulkan.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <vulkan/vulkan_core.h>

#include "render/context.h"

namespace tire::vk {

struct Pipeline;

struct ContextBare final : Context {
    ContextBare( Display* display, Window window );
    ~ContextBare();

    ContextBare( const ContextBare& other ) = delete;
    ContextBare( Context&& other ) = delete;
    auto operator=( const ContextBare& other ) -> ContextBare& = delete;
    auto operator=( ContextBare&& other ) -> ContextBare& = delete;

    // Init all context
    void init();

    [[nodiscard]] auto getFrameSyncSet( size_t id )
        -> std::tuple<VkSemaphore, VkSemaphore, VkFence, VkCommandBuffer> {
        return { frames_[id].imageAvailableSemaphore_,
                 frames_[id].renderFinishedSemaphore_,
                 frames_[id].inFlightFence_, frames_[id].cbPrimary_ };
    }

    [[nodiscard]] VkFramebuffer framebuffer( size_t id ) const {
        return frames_[id].framebuffer_;
    };

    [[nodiscard]] auto presentQueue() const -> VkQueue {
        //
        return presentQueue_;
    };

    auto renderCommandBegin( uint32_t frameId ) -> void;
    auto renderCommandEnd( uint32_t frameId ) -> void;

    [[nodiscard]]
    auto getDrawCommandBuffer( size_t id ) const -> VkCommandBuffer {
        //
        return frames_[id].cbPrimary_;
    }

    [[nodiscard]] auto framesCount() const -> uint32_t {
        //
        return framesCount_;
    };

    [[nodiscard]] auto commandPool() const -> VkCommandPool {
        return commandPool_;
    };

private:
    auto makeInstance() -> void;
    auto makeXlibSurface() -> void;
    auto collectPhysicalDevices() -> void;
    auto makeDevice() -> void;
    auto makeCommandPool() -> void;
    auto makeSwapchain() -> void;
    auto initRenderPass() -> void;
    auto makeFrames() -> void;

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
        VkCommandBuffer cbPrimary_{ VK_NULL_HANDLE };
        VkCommandBuffer cbSecondary_{ VK_NULL_HANDLE };
    };

private:
    // Outer world connection
    const Display* display_;
    const Window window_;

    // Instance
    VkDebugUtilsMessengerEXT debugMessenger_{ VK_NULL_HANDLE };
    std::vector<const char*> desiredValidationLayerList_{};
    std::vector<VkExtensionProperties> extensionProperties_{};
    std::vector<VkLayerProperties> layerProperties_{};

    // Physical and logical devices
    std::vector<PhysicalDevice> physicalDevices_{};
    int pickedPhysicalDeviceId_{ -1 };
    uint32_t presentSupportQueueId_{ UINT32_MAX };
    VkQueue presentQueue_{ VK_NULL_HANDLE };
    VkSurfaceCapabilitiesKHR surfaceCapabilities_{};
    std::vector<VkSurfaceFormatKHR> surfaceFormats_{};
    std::vector<VkPresentModeKHR> presentModes_{};
    VkPresentModeKHR presentMode_{};

    // Command pool
    VkCommandPool commandPool_{ VK_NULL_HANDLE };

    // Background color value
    std::array<VkClearValue, 2> clearValues_{};

    // Swapchain
    uint32_t swapchainImageCount_{};
    uint32_t framesCount_{};
    std::vector<Frame> frames_{};
    VkImage depthImage_;
    VkDeviceMemory depthImageMemory_;
    VkImageView depthImageView_;
};

}  // namespace tire::vk