
#pragma once

#include <atomic>
#include <mutex>
#include <format>
#include <memory>
#include <vector>

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

#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include "vma/vk_mem_alloc.h"

#ifdef SURFACE_X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#elifdef SURFACE_WAYLAND
#include <wayland-client.h>
#endif

#include "config/config.h"
#include "log/log.h"
#include "image/image.h"

#include "command_routine.h"

namespace tire {

struct DepthImage;

struct Context final {
    Context( const Context &other ) = delete;
    Context( Context &&other ) = delete;

    auto operator=( const Context &other ) -> Context & = delete;
    auto operator=( Context &&other ) -> Context & = delete;

    static void init( uint32_t width, uint32_t height, Display *display, Window window ) {
        // std::call_once guarantees the lambda is executed exactly once,
        // even if multiple threads call init() concurrently.
        std::call_once( init_flag_, [&]() {
            // We deliberately use 'new' and do not delete.
            // This is intentional. It solves the Static Destruction Order Fiasco.
            // If your Singleton is destroyed during program shutdown, and another static
            // object's destructor tries to use it, your program crashes. By leaking
            // the pointer, the Singleton survives past the end of the program, and the OS automatically
            // reclaims the memory when the process exits anyway.
            _instance.store( new Context( width, height, display, window ) );
        } );

        // Optional: Warn or throw if init is called again with different arguments
        // if ( _instance.load()a_ != a ||
        //      _instance.load()b_ != b ) {
        //     std::cerr << "Warning: Singleton already initialized. Ignoring new arguments.\n";
        // }
    }

    [[nodiscard]] static Context &instance() {
        // memory_order_acquire ensures we see the fully constructed object
        Context *ptr = _instance.load();

        if ( !ptr ) {
            throw std::logic_error( "Singleton must be initialized via init() before calling getInstance()." );
        }
        return *ptr;
    }

#ifdef SURFACE_X11
    auto makeXlibSurface( Display *display, Window window ) -> void;
#elifdef SURFACE_WAYLAND
    auto makeWaylandSurface( wl_display *display, wl_surface *surface ) -> void;
#endif

    [[nodiscard]] auto vkInstance() const -> VkInstance {
        //
        return vkInstance_;
    }

    [[nodiscard]] auto surface() const -> VkSurfaceKHR {
        //
        return surface_;
    }

    [[nodiscard]] auto device() const -> VkDevice {
        //
        return device_;
    }

    [[nodiscard]] auto swapchain() const -> VkSwapchainKHR {
        //
        return swapchain_;
    }

    [[nodiscard]] auto surfaceFormat() const -> const VkSurfaceFormatKHR & { return surfaceFormat_; };

    [[nodiscard]] auto graphicsQueue() const -> const VkQueue & { return graphicsQueue_; }

    [[nodiscard]] auto graphicsFamily() const -> uint32_t {
        //
        return graphicsFamilyQueueId_;
    }

    [[nodiscard]] auto physicalDevice() const -> VkPhysicalDevice {
        //
        return physDevice_;
    }

    [[nodiscard]] auto currentExtent() const -> const VkExtent2D & { return currentExtent_; };

    [[nodiscard]] auto viewportSize() -> std::tuple<uint32_t, uint32_t> const { return { width_, height_ }; }

    [[nodiscard]] auto memoryRequirements( uint32_t typeFilter, VkMemoryPropertyFlags properties ) const -> uint32_t;

    [[nodiscard]] auto findSupportedFormat( const std::vector<VkFormat> &candidates, VkImageTiling tiling,
                                            VkFormatFeatureFlags features ) const -> VkFormat;

    [[nodiscard]] auto renderPass() const -> VkRenderPass {
        //
        return renderPass_;
    };

    [[nodiscard]] auto allocator() const -> VmaAllocator {
        //
        return allocator_;
    }

    [[nodiscard]] auto getFrameSyncSet( size_t id ) -> std::tuple<VkSemaphore, VkSemaphore, VkFence, VkCommandBuffer> {
        return { frames_[id].imageAvailableSemaphore_, frames_[id].renderFinishedSemaphore_, frames_[id].inFlightFence_,
                 frames_[id].cbPrimary_ };
    }

    [[nodiscard]] auto framebuffer( size_t id ) const -> VkFramebuffer {
        //
        return frames_[id].framebuffer_;
    };

    [[nodiscard]] auto presentQueue() const -> VkQueue {
        //
        return presentQueue_;
    };

    auto renderCommandBegin( uint32_t frameId ) -> void;
    auto renderCommandEnd( uint32_t frameId ) -> void;

    [[nodiscard]] auto getDrawCommandBuffer( size_t id ) const -> VkCommandBuffer {
        //
        return frames_[id].cbPrimary_;
    }

    [[nodiscard]] auto framesCount() const -> uint32_t {
        //
        return framesCount_;
    };

    [[nodiscard]] auto commandPool() const -> VkCommandPool {
        //
        return commandPool_;
    };

    [[nodiscard]] auto descriptorPool() const -> VkDescriptorPool {
        //
        return descriptorPool_;
    };

    [[nodiscard]] auto renderCommand( uint32_t frameId ) -> CommandRoutine;

    [[nodiscard]] auto copyBufferCommand() const -> CommandRoutine;

    [[nodiscard]] auto immediateCommand() const -> CommandRoutine;

private:
#ifdef SURFACE_X11
    Context( uint32_t width, uint32_t height, Display *display, Window window );
#elifdef SURFACE_WAYLAND
    Context( uint32_t width, uint32_t height, wl_display *display, wl_surface *surface );
#endif

    // Destroy all Vulkan context here.
    ~Context();

    inline static std::atomic<Context *> _instance{ nullptr };
    inline static std::once_flag init_flag_;

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
    // Init all context
    auto makeInstance( const std::string &platformSurfaceExtension ) -> void;
    auto collectPhysicalDevices() -> void;
    auto makeDevice() -> void;
    auto makeCommandPool() -> void;
    auto makeSwapchain() -> void;
    auto initRenderPass() -> void;
    auto makeFrames() -> void;
    auto createAllocator() -> void;
    auto createDescriptorPool() -> void;
    auto initCopyCommandBuffer() -> void;

    auto initRest() -> void {
        collectPhysicalDevices();
        makeDevice();
        createAllocator();
        makeCommandPool();
        makeSwapchain();
        initRenderPass();
        makeFrames();
        createDescriptorPool();
        initCopyCommandBuffer();

        // Note that the order of clearValues should be identical to the order of your
        // attachments
        const auto configHandle = tire::Config::instance();
        const auto colorString = configHandle->get<std::string>( "background_color" );
        const auto backgroundColor = Colorf( colorString );
        clearValues_[0].color = { { backgroundColor.r(), backgroundColor.g(), backgroundColor.b(), 1.0f } };
        clearValues_[1].depthStencil = { .depth = 1.0f, .stencil = 0 };
    };

    auto pickDevice( const std::vector<PhysicalDevice> &physDevList ) -> std::optional<int>;

protected:
    // Instance
    VkInstance vkInstance_{ VK_NULL_HANDLE };
    VkDebugUtilsMessengerEXT debugMessenger_{ VK_NULL_HANDLE };
    std::vector<const char *> desiredValidationLayerList_{};
    std::vector<VkExtensionProperties> extensionProperties_{};
    std::vector<VkLayerProperties> layerProperties_{};

    // Surface
    VkSurfaceKHR surface_{ VK_NULL_HANDLE };
    VkSurfaceFormatKHR surfaceFormat_{};

    // Physical and logical devices
    std::vector<PhysicalDevice> physicalDevices_{};
    int pickedPhysicalDeviceId_{ -1 };
    uint32_t presentSupportQueueId_{ UINT32_MAX };
    VkQueue presentQueue_{ VK_NULL_HANDLE };
    VkSurfaceCapabilitiesKHR surfaceCapabilities_{};
    std::vector<VkSurfaceFormatKHR> surfaceFormats_{};
    std::vector<VkPresentModeKHR> presentModes_{};
    VkPresentModeKHR presentMode_{};
    VkPhysicalDevice physDevice_{};
    // The logical device itself.
    VkDevice device_{ VK_NULL_HANDLE };

    // Swapchain
    VkSwapchainKHR swapchain_{ VK_NULL_HANDLE };
    uint32_t framesCount_{};
    uint32_t swapchainImageCount_{};
    std::vector<Frame> frames_{};
    std::shared_ptr<DepthImage> depthImage_;

    VkQueue graphicsQueue_{ VK_NULL_HANDLE };
    uint32_t graphicsFamilyQueueId_{ UINT32_MAX };

    VkRenderPass renderPass_{ VK_NULL_HANDLE };

    VkExtent2D currentExtent_{};
    uint32_t width_{};
    uint32_t height_{};

    // Command pool
    VkCommandPool commandPool_{ VK_NULL_HANDLE };

    // Background color value
    std::array<VkClearValue, 2> clearValues_{};

    // Momory
    VmaAllocator allocator_{};

    //
    VkDescriptorPool descriptorPool_{};

    // Reusable command buffer with fence.
    VkFence copyCommandFence_{};
    VkCommandBuffer copyCommandBuffer_{};
};

}  // namespace tire
