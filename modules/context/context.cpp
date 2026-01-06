
module;

#include <format>
#include <vector>
#include <memory>

#define SURFACE_X11

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

export module context:context;

import log;
import config;
import image;

namespace tire {

export struct CommandRoutine;
struct DepthImage;

auto vkDestroyDebugUtilsMessenger(
  VkInstance instance, VkDebugUtilsMessengerEXT messanger, const VkAllocationCallbacks *pAllocator ) -> void {
    auto func =
      (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr( instance, "vkDestroyDebugUtilsMessengerEXT" );
    if ( func != nullptr ) {
        return func( instance, messanger, pAllocator );
    }
}

export struct Context final {
#ifdef SURFACE_X11
    Context( uint32_t width, uint32_t height, Display *display, Window window )
        : width_{ width }
        , height_{ height } {
        //
        makeInstance( "VK_KHR_xlib_surface" );
        makeXlibSurface( display, window );
        initRest();
    }
#elifdef SURFACE_WAYLAND
    Context( uint32_t width, uint32_t height, wl_display *display, wl_surface *surface )
        : width_{ width }
        , height_{ height } {
        //
        makeInstance( "VK_KHR_wayland_surface" );
        makeWaylandSurface( display, surface );
        initRest();
    }
#endif

    Context( const Context &other ) = delete;
    Context( Context &&other ) = delete;

    auto operator=( const Context &other ) -> Context & = delete;
    auto operator=( Context &&other ) -> Context & = delete;

    // Destroy all Vulkan context here.
    ~Context() {
        vkDestroyFence( device_, copyCommandFence_, nullptr );

        vkDestroyDescriptorPool( device_, descriptorPool_, nullptr );

        for ( auto i = 0; i < framesCount_; i++ ) {
            vkDestroySemaphore( device_, frames_[i].imageAvailableSemaphore_, nullptr );
            vkDestroySemaphore( device_, frames_[i].renderFinishedSemaphore_, nullptr );
            vkDestroyFence( device_, frames_[i].inFlightFence_, nullptr );
            vkDestroyFramebuffer( device_, frames_[i].framebuffer_, nullptr );
            vkDestroyImageView( device_, frames_[i].view_, nullptr );
        }

        vkDestroyCommandPool( device_, commandPool_, nullptr );

        vkDestroySwapchainKHR( device_, swapchain_, nullptr );

        vkDestroyDevice( device_, nullptr );
        vkDestroySurfaceKHR( instance_, surface_, nullptr );

        vkDestroyDebugUtilsMessenger( instance_, debugMessenger_, nullptr );

        vmaDestroyAllocator( allocator_ );

        vkDestroyInstance( instance_, nullptr );
    }

#ifdef SURFACE_X11
    auto makeXlibSurface( Display *display, Window window ) -> void {
        VkXlibSurfaceCreateInfoKHR xlibSurfInfo{};
        xlibSurfInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        xlibSurfInfo.dpy = display;
        xlibSurfInfo.window = window;

        if ( const auto err = vkCreateXlibSurfaceKHR( instance_, &xlibSurfInfo, nullptr, &surface_ );
             err != VK_SUCCESS ) {
            throw std::runtime_error(
              std::format( "failed to create xlib surface with code {}\n!", string_VkResult( err ) ) );
        } else {
            log::info( "Surface === xlib surface created!" );
        }
    }
#elifdef SURFACE_WAYLAND
    auto makeWaylandSurface( wl_display *display, wl_surface *surface ) -> void {
        VkWaylandSurfaceCreateInfoKHR wlSurfInfo{};
        wlSurfInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
        wlSurfInfo.display = display;
        wlSurfInfo.surface = surface;

        if ( const auto err = vkCreateWaylandSurfaceKHR( instance_, &wlSurfInfo, nullptr, &surface_ );
             err != VK_SUCCESS ) {
            throw std::runtime_error(
              std::format( "failed to create wayland surface with code {}\n!", string_VkResult( err ) ) );
        } else {
            log::info( "Surface === xlib surface created!" );
        }
    }
#endif

    [[nodiscard]] auto instance() const -> VkInstance {
        //
        return instance_;
    };

    [[nodiscard]] auto surface() const -> VkSurfaceKHR {
        //
        return surface_;
    };

    [[nodiscard]] auto device() const -> VkDevice {
        //
        return device_;
    };

    [[nodiscard]] auto swapchain() const -> VkSwapchainKHR {
        //
        return swapchain_;
    };

    [[nodiscard]]
    auto surfaceFormat() const -> const VkSurfaceFormatKHR & {
        return surfaceFormat_;
    };

    [[nodiscard]]
    auto graphicsQueue() const -> const VkQueue & {
        return graphicsQueue_;
    }

    [[nodiscard]] auto graphicsFamily() const -> uint32_t {
        //
        return graphicsFamilyQueueId_;
    };

    [[nodiscard]]
    auto physicalDevice() const -> VkPhysicalDevice {
        return physDevice_;
    }

    [[nodiscard]]
    auto currentExtent() const -> const VkExtent2D & {
        return currentExtent_;
    };

    [[nodiscard]]
    auto viewportSize() -> std::tuple<uint32_t, uint32_t> const {
        return { width_, height_ };
    }

    [[nodiscard]]
    auto memoryRequirements( uint32_t typeFilter, VkMemoryPropertyFlags properties ) const -> uint32_t;

    [[nodiscard]]
    auto findSupportedFormat(
      const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features ) const -> VkFormat;

    [[nodiscard]] auto renderPass() const -> VkRenderPass {
        //
        return renderPass_;
    };

    [[nodiscard]] auto allocator() const -> VmaAllocator {
        //
        return allocator_;
    }

    [[nodiscard]] auto getFrameSyncSet( size_t id ) -> std::tuple<VkSemaphore, VkSemaphore, VkFence, VkCommandBuffer> {
        return {
          frames_[id].imageAvailableSemaphore_, frames_[id].renderFinishedSemaphore_, frames_[id].inFlightFence_,
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
        //
        return commandPool_;
    };

    [[nodiscard]] auto descriptorPool() const -> VkDescriptorPool {
        //
        return descriptorPool_;
    };

    [[nodiscard]]
    auto renderCommand( uint32_t frameId ) -> CommandRoutine;

    [[nodiscard]]
    auto copyBufferCommand() const -> CommandRoutine;

    [[nodiscard]]
    auto immidiateCommand() const -> CommandRoutine;

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

protected:
    // Instance
    VkInstance instance_{ VK_NULL_HANDLE };
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
    VkDevice device_{ VK_NULL_HANDLE };

    // Swapchain
    VkSwapchainKHR swapchain_{ VK_NULL_HANDLE };
    uint32_t framesCount_{};
    uint32_t swapchainImageCount_{};
    std::vector<Frame> frames_{};
    std::shared_ptr<DepthImage> depthImage_;

    VkQueue graphicsQueue_{ VK_NULL_HANDLE };
    VkRenderPass renderPass_{ VK_NULL_HANDLE };

    uint32_t graphicsFamilyQueueId_{ UINT32_MAX };

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
