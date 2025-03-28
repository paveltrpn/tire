
#include <stdexcept>
#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vk_enum_string_helper.h>

#include "context.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_CONTEXT_CPP{ true };

#include "pipelines/pipeline.h"

namespace tire::vk {

Context::Context( Display *display, Window window )
    : display_{ display }
    , window_{ window } {
}

void Context::init() {
    makeInstance();
    makeSurface();
    collectPhysicalDevices();
    makeDevice();
    makeCommandPool();
    makeSwapchain();
}

void Context::makeSurface() {
    VkXlibSurfaceCreateInfoKHR xlibSurfInfo_{};
    xlibSurfInfo_.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    xlibSurfInfo_.dpy = const_cast<Display *>( display_ );
    xlibSurfInfo_.window = window_;

    if ( const auto err = vkCreateXlibSurfaceKHR( instance_, &xlibSurfInfo_,
                                                  nullptr, &surface_ );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create xlib surface with code {}\n!",
                         string_VkResult( err ) ) );
    } else {
        log::info( "vk::Surface === xlib surface created!" );
    }
}

uint32_t Context::memoryRequirements( uint32_t typeFilter,
                                      VkMemoryPropertyFlags properties ) const {
    VkPhysicalDeviceMemoryProperties memProperties{};
    vkGetPhysicalDeviceMemoryProperties(
        physicalDevices_[pickedPhysicalDeviceId_].device, &memProperties );

    for ( uint32_t i = 0; i < memProperties.memoryTypeCount; i++ ) {
        if ( ( typeFilter & ( 1 << i ) ) &&
             ( memProperties.memoryTypes[i].propertyFlags & properties ) ==
                 properties ) {
            return i;
        }
    }

    log::fatal( "failed to find suitable memory type!" );

    // Silence warning
    return {};
}

VkFormat Context::findSupportedFormat( const std::vector<VkFormat> &candidates,
                                       VkImageTiling tiling,
                                       VkFormatFeatureFlags features ) const {
    for ( VkFormat format : candidates ) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(
            physicalDevices_[pickedPhysicalDeviceId_].device, format, &props );

        if ( tiling == VK_IMAGE_TILING_LINEAR &&
             ( props.linearTilingFeatures & features ) == features ) {
            return format;
        } else if ( tiling == VK_IMAGE_TILING_OPTIMAL &&
                    ( props.optimalTilingFeatures & features ) == features ) {
            return format;
        }
    }

    throw std::runtime_error(
        std::format( "failed to find supported format!" ) );

    // Silence warning
    return {};
}

void Context::makeCommandPool() {
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

void Context::present( const VkSemaphore semaphore, uint32_t *imageIndex ) {
    std::array<VkSwapchainKHR, 1> swapChains = { swapchain_ };
    std::array<VkSemaphore, 1> signalSemaphores = { semaphore };

    const VkPresentInfoKHR presentInfo{
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = nullptr,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = signalSemaphores.data(),
        .swapchainCount = 1,
        .pSwapchains = swapChains.data(),
        .pImageIndices = imageIndex,
        .pResults = nullptr };

    // May return VK_SUBOPTIMAL_KHR or even VK_ERROR_OUT_OF_DATE_KHR
    // if current surface properties are no longer matched
    // exactly or swap chain has become incompatible
    // with the surface and can no longer be used for rendering
    if ( const auto res = vkQueuePresentKHR( presentQueue_, &presentInfo );
         res != VK_SUCCESS ) {
        log::warning( "queue present result {}, is it normal?",
                      string_VkResult( res ) );
    }
}

Context::~Context() {
    for ( auto i = 0; i < framesCount_; i++ ) {
        vkDestroySemaphore( device_, frames_[i].imageAvailableSemaphore_,
                            nullptr );
        vkDestroySemaphore( device_, frames_[i].renderFinishedSemaphore_,
                            nullptr );
        vkDestroyFence( device_, frames_[i].inFlightFence_, nullptr );
        vkDestroyFramebuffer( device_, frames_[i].framebuffer_, nullptr );
    }

    vkDestroySwapchainKHR( device_, swapchain_, nullptr );

    vkDestroyDevice( device_, nullptr );
    vkDestroySurfaceKHR( instance_, surface_, nullptr );
    vkDestroyInstance( instance_, nullptr );
}

}  // namespace tire::vk