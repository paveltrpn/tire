
#pragma once

#include <atomic>
#include <mutex>
#include <memory>
#include <vector>
#include <array>

#define SURFACE_X11
// #define SURFACE_WAYLAND

#ifdef SURFACE_X11
#define VK_USE_PLATFORM_XLIB_KHR
#elifdef SURFACE_WAYLAND
#define VK_USE_PLATFORM_WAYLAND_KHR
#endif

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#ifdef SURFACE_X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#elifdef SURFACE_WAYLAND
#include <wayland-client.h>
#endif

#include "vkinstance.h"
#include "surface.h"
#include "device.h"
#include "allocator.h"
#include "presentation.h"
#include "contextpools.h"

#include "command_routine.h"

namespace tire {

struct DepthImage;

struct Context final {
    Context( const Context& other ) = delete;
    Context( Context&& other ) = delete;

    auto operator=( const Context& other ) -> Context& = delete;
    auto operator=( Context&& other ) -> Context& = delete;

    static void init( uint32_t width, uint32_t height, Display* display, Window window );
    [[nodiscard]]
    static auto instance() -> Context&;

    [[nodiscard]]
    auto vkInstance() const -> VkInstance {
        //
        return _vkInstance->get();
    }

    [[nodiscard]]
    auto surface() const -> VkSurfaceKHR {
        //
        return _vkSurface->get();
    }

    [[nodiscard]] auto device() const -> VkDevice {
        //
        return _vkDevice->get();
    }

    [[nodiscard]] auto swapchain() const -> VkSwapchainKHR {
        //
        return _swapchain;
    }

    [[nodiscard]] auto surfaceFormat() const -> const VkSurfaceFormatKHR& {
        //
        return _vkSurface->surfaceFormat();
    };

    [[nodiscard]] auto graphicsQueue() const -> VkQueue {
        //
        return _vkDevice->graphicsQueue();
    }

    [[nodiscard]] auto presentQueue() const -> VkQueue {
        //
        return _vkDevice->presentQueue();
    }

    [[nodiscard]] auto graphicsFamilyQueueId() const -> uint32_t {
        //
        return _vkDevice->graphicsFamilyQueueId();
    }

    [[nodiscard]] auto presentSupportQueueId() const -> uint32_t {
        //
        return _vkDevice->presentSupportQueueId();
    }

    [[nodiscard]] auto physicalDevice() const -> VkPhysicalDevice {
        //
        return _vkDevice->physicalDevice();
    }

    [[nodiscard]] auto currentExtent() const -> const VkExtent2D& {
        //
        return _vkSurface->currentExtent();
    };

    [[nodiscard]] auto viewportSize() -> std::tuple<uint32_t, uint32_t> const {
        //
        return { _width, _height };
    }

    [[nodiscard]] auto presentMode() const -> VkPresentModeKHR {
        //
        return _vkSurface->presentMode();
    }

    auto surfaceCapabilities() const -> const VkSurfaceCapabilitiesKHR& {
        //
        return _vkSurface->surfaceCapabilities();
    }

    [[nodiscard]] auto memoryRequirements( uint32_t typeFilter, VkMemoryPropertyFlags properties ) const
        -> std::optional<uint32_t>;
    [[nodiscard]] auto findSupportedFormat( const std::vector<VkFormat>& candidates, VkImageTiling tiling,
                                            VkFormatFeatureFlags features ) const -> std::optional<VkFormat>;

    [[nodiscard]] auto renderPass() const -> VkRenderPass {
        //
        return _renderPass;
    };

    [[nodiscard]] auto allocator() const -> VmaAllocator {
        //
        return _allocator->get();
    }

    [[nodiscard]] auto getFrameSyncSet( size_t id ) -> std::tuple<VkSemaphore, VkSemaphore, VkFence, VkCommandBuffer> {
        return { _frames[id]._imageAvailableSemaphore, _frames[id]._renderFinishedSemaphore, _frames[id]._inFlightFence,
                 _frames[id]._cbPrimary };
    }

    [[nodiscard]] auto framebuffer( size_t id ) const -> VkFramebuffer {
        //
        return _frames[id]._framebuffer;
    };

    auto renderCommandBegin( uint32_t frameId ) -> void;
    auto renderCommandEnd( uint32_t frameId ) -> void;

    [[nodiscard]] auto getDrawCommandBuffer( size_t id ) const -> VkCommandBuffer {
        //
        return _frames[id]._cbPrimary;
    }

    [[nodiscard]] auto framesCount() const -> uint32_t {
        //
        return _framesCount;
    };

    [[nodiscard]] auto commandPool() const -> VkCommandPool {
        //
        return _contextPools->commandPool();
    };

    [[nodiscard]] auto descriptorPool() const -> VkDescriptorPool {
        //
        return _contextPools->descriptorPool();
    };

    [[nodiscard]] auto renderCommand( uint32_t frameId ) -> CommandRoutine;
    [[nodiscard]] auto copyBufferCommand() const -> CommandRoutine;
    [[nodiscard]] auto immediateCommand() const -> CommandRoutine;

    // Destroy all Vulkan context here.
    auto releaseContext() -> void;

private:
#ifdef SURFACE_X11
    Context( uint32_t width, uint32_t height, Display* display, Window window );
#elifdef SURFACE_WAYLAND
    Context( uint32_t width, uint32_t height, wl_display* display, wl_surface* surface );
#endif

    ~Context() = default;

    inline static std::atomic<Context*> _instance{ nullptr };
    inline static std::once_flag _initFlag;
    inline static bool _initSuccess{ false };

private:
    struct Frame final {
        VkImage _image{};
        VkImageView _view{};
        VkFramebuffer _framebuffer{};
        VkSemaphore _imageAvailableSemaphore{};
        VkSemaphore _renderFinishedSemaphore{};
        VkFence _inFlightFence{};
        VkCommandBuffer _cbPrimary{ VK_NULL_HANDLE };
        VkCommandBuffer _cbSecondary{ VK_NULL_HANDLE };
    };

private:
    auto makeSwapchain() -> void;
    auto initRenderPass() -> void;
    auto makeFrames() -> void;
    auto initCopyCommandBuffer() -> void;

protected:
    std::unique_ptr<VKInstance> _vkInstance{};
    std::unique_ptr<VKDevice> _vkDevice{};
    std::unique_ptr<VKSurface> _vkSurface{};
    std::unique_ptr<VMAllocator> _allocator{};
    std::unique_ptr<Presentation> _presentation{};
    std::unique_ptr<ContextPools> _contextPools{};

    // Swapchain
    VkSwapchainKHR _swapchain{ VK_NULL_HANDLE };
    uint32_t _framesCount{};
    uint32_t _swapchainImageCount{};
    std::vector<Frame> _frames{};
    std::shared_ptr<DepthImage> _depthImage;

    VkRenderPass _renderPass{ VK_NULL_HANDLE };

    uint32_t _width{};
    uint32_t _height{};

    // Background color value
    std::array<VkClearValue, 2> _clearValues{};

    // Reusable command buffer with fence.
    VkFence _copyCommandFence{};
    VkCommandBuffer _copyCommandBuffer{};
};

}  // namespace tire
