
#include <stdexcept>

#define SURFACE_X11

#ifdef SURFACE_X11
#define VK_USE_PLATFORM_XLIB_KHR
#elifdef SURFACE_WAYLAND
#define VK_USE_PLATFORM_WAYLAND_KHR
#endif
#include "vulkan/vulkan_core.h"
#include <vulkan/vk_enum_string_helper.h>

#include "config/config.h"
#include "context_bare.h"
#include "image/color.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_CONTEXT_CPP{ true };

namespace tire::vk {

static void vkDestroyDebugUtilsMessenger(
    VkInstance instance, VkDebugUtilsMessengerEXT messanger,
    const VkAllocationCallbacks* pAllocator ) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkDestroyDebugUtilsMessengerEXT" );
    if ( func != nullptr ) {
        return func( instance, messanger, pAllocator );
    }
}

auto ContextBare::init() -> void {
    collectPhysicalDevices();
    makeDevice();
    makeCommandPool();
    makeSwapchain();
    initRenderPass();
    makeFrames();

    // Note that the order of clearValues should be identical to the order of your
    // attachments
    const auto configHandle = tire::Config::instance();
    const auto colorString =
        configHandle->get<std::string>( "background_color" );
    const auto backgroundColor = Colorf( colorString );
    clearValues_[0].color = { { backgroundColor.r(), backgroundColor.g(),
                                backgroundColor.b(), 1.0f } };
    clearValues_[1].depthStencil = { .depth = 1.0f, .stencil = 0 };
}

#ifdef SURFACE_X11
auto ContextBare::makeXlibSurface( Display* display, Window window ) -> void {
    VkXlibSurfaceCreateInfoKHR xlibSurfInfo{};
    xlibSurfInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    xlibSurfInfo.dpy = display;
    xlibSurfInfo.window = window;

    if ( const auto err = vkCreateXlibSurfaceKHR( instance_, &xlibSurfInfo,
                                                  nullptr, &surface_ );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create xlib surface with code {}\n!",
                         string_VkResult( err ) ) );
    } else {
        log::info( "vk::Surface === xlib surface created!" );
    }
}
#elifdef SURFACE_WAYLAND
auto ContextBare::makeWaylandSurface( wl_display* display, wl_surface* surface )
    -> void {
    VkWaylandSurfaceCreateInfoKHR wlSurfInfo{};
    wlSurfInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    wlSurfInfo.display = display;
    wlSurfInfo.surface = surface;

    if ( const auto err = vkCreateWaylandSurfaceKHR( instance_, &wlSurfInfo,
                                                     nullptr, &surface_ );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create wayland surface with code {}\n!",
                         string_VkResult( err ) ) );
    } else {
        log::info( "vk::Surface === xlib surface created!" );
    }
}
#endif

void ContextBare::makeCommandPool() {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = graphicsFamilyQueueId_;

    if ( const auto err =
             vkCreateCommandPool( device_, &poolInfo, nullptr, &commandPool_ );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create command pool woth code {}!",
                         string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_CONTEXT_CPP>(
            "vk::CommandPool === command pool created!" );
    }
}

ContextBare::~ContextBare() {
    vkDestroyImage( device_, depthImage_, nullptr );
    vkDestroyImageView( device_, depthImageView_, nullptr );
    vkFreeMemory( device_, depthImageMemory_, nullptr );

    for ( auto i = 0; i < framesCount_; i++ ) {
        vkDestroySemaphore( device_, frames_[i].imageAvailableSemaphore_,
                            nullptr );
        vkDestroySemaphore( device_, frames_[i].renderFinishedSemaphore_,
                            nullptr );
        vkDestroyFence( device_, frames_[i].inFlightFence_, nullptr );
        vkDestroyFramebuffer( device_, frames_[i].framebuffer_, nullptr );
        vkDestroyImageView( device_, frames_[i].view_, nullptr );
    }

    vkDestroyCommandPool( device_, commandPool_, nullptr );

    vkDestroySwapchainKHR( device_, swapchain_, nullptr );

    vkDestroyDevice( device_, nullptr );
    vkDestroySurfaceKHR( instance_, surface_, nullptr );

    vkDestroyDebugUtilsMessenger( instance_, debugMessenger_, nullptr );

    vkDestroyInstance( instance_, nullptr );
}

auto ContextBare::initRenderPass() -> void {
    VkAttachmentDescription colorAttachment{};
    // Same as swapchain image format
    colorAttachment.format = surfaceFormat().format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    //Similar to swapchain creation depth format acquire!
    const auto depthFormat = findSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
          VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT );
    // const auto depthFormat = VK_FORMAT_D32_SFLOAT;

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = depthFormat;
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                              VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                              VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                               VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = { colorAttachment,
                                                           depthAttachment };

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount =
        static_cast<uint32_t>( attachments.size() );
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if ( const auto err = vkCreateRenderPass( device(), &renderPassInfo,
                                              nullptr, &renderPass_ );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create render pass with code {}!",
                         string_VkResult( err ) ) );
    } else {
        log::info( "vk::PipelineVertexBuffer === render pass created!" );
    }
}

}  // namespace tire::vk