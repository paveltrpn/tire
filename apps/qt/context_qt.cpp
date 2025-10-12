

#include "vulkan/vulkan.h"
#include <vulkan/vk_enum_string_helper.h>

#include "context_qt.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_CONTEXT_CPP{ true };

namespace tire::vk {

ContextQt::ContextQt( VkInstance instance, VkPhysicalDevice pDevice,
                      VkDevice device, VkSurfaceKHR surface, VkRenderPass rp,
                      uint32_t gqfi, uint32_t gqi )
    : pDevice_{ pDevice }
    , renderPass_{ rp }
    , graphicsFamilyQueueId_{ gqfi }
    , graphicsQueueId_{ gqi } {
    instance_ = instance;
    device_ = device;
    surface_ = surface;
    uint32_t version{};
    vkEnumerateInstanceVersion( &version );
    log::info( "vk::Context === acquired api instance version is {}.{}",
               VK_API_VERSION_MAJOR( version ),
               VK_API_VERSION_MINOR( version ) );

    queryDeviceInfo();
    querySurface();

    // Graphic queue.
    vkGetDeviceQueue( device_, graphicsFamilyQueueId_, graphicsQueueId_,
                      &graphicsQueue_ );
}

auto ContextQt::queryDeviceInfo() -> void {
    // Collect physical devices and its properties
    vkGetPhysicalDeviceProperties( pDevice_, &pDeviceProperties_ );
    log::info( "vk::Device === name: {}", pDeviceProperties_.deviceName );

    vkGetPhysicalDeviceFeatures( pDevice_, &pDeviceFeatures_ );

    // Collect physical device queue families properies
    uint32_t queueFamilyCount{};
    vkGetPhysicalDeviceQueueFamilyProperties( pDevice_, &queueFamilyCount,
                                              nullptr );

    queueFamilyProperties_.resize( queueFamilyCount );
    vkGetPhysicalDeviceQueueFamilyProperties( pDevice_, &queueFamilyCount,
                                              queueFamilyProperties_.data() );
    log::debug<DEBUG_OUTPUT_CONTEXT_CPP>(
        "vk::Device === device queue family count: {}", queueFamilyCount );

    // Collect physical device extensions info
    uint32_t extensionCount{};
    if ( const auto err = vkEnumerateDeviceExtensionProperties(
             pDevice_, nullptr, &extensionCount, nullptr );
         err != VK_SUCCESS ) {
        log::fatal(
            "can't enumerate physical device extensions "
            "with code: {}\n",
            string_VkResult( err ) );
    } else {
        log::debug<DEBUG_OUTPUT_CONTEXT_CPP>(
            "vk::Device === physical device extensions enumerated for "
            "device: {}, "
            "count: {}",
            pDeviceProperties_.deviceName, extensionCount );
    }

    pDeviceExtensions_.resize( extensionCount );
    if ( const auto err = vkEnumerateDeviceExtensionProperties(
             pDevice_, nullptr, &extensionCount, pDeviceExtensions_.data() );
         err != VK_SUCCESS ) {
        log::fatal(
            "can't acquire physical device extensions "
            "with code: {}\n",
            string_VkResult( err ) );
    } else {
        log::debug<DEBUG_OUTPUT_CONTEXT_CPP>(
            "vk::Device === physical device extensions acquired for "
            "device: "
            "{}",
            pDeviceProperties_.deviceName );
    }

    log::info(
        "vk::Context === vulkan "
        "info:\ndev:\t{}\ndrv:\t{}\napi:\t{}",
        getDeviceNameString(), getDeviceDriverVersionString(),
        getApiVersionString() );
}

auto ContextQt::querySurface() -> void {
    if ( surface_ == VK_NULL_HANDLE ) {
        log::error( "Context === can not get surface handle for window!" );
    }

    // Physical device surface capabilities.
    // NOTE: got error on amd APU.
    if ( const auto err = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
             pDevice_, surface_, &surfaceCapabilities_ );
         err != VK_SUCCESS ) {
        log::fatal(
            "vk::Context === failed to obtain surface capabilities with code "
            "{}!\n",
            string_VkResult( err ) );
    } else {
        log::debug<DEBUG_OUTPUT_CONTEXT_CPP>(
            "vk::Context === physical device surface capabilities acquire "
            "success!" );
    }
}

auto ContextQt::memoryRequirements( uint32_t typeFilter,
                                    VkMemoryPropertyFlags properties ) const
    -> uint32_t {
    VkPhysicalDeviceMemoryProperties memProperties{};
    vkGetPhysicalDeviceMemoryProperties( pDevice_, &memProperties );

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

auto ContextQt::findSupportedFormat( const std::vector<VkFormat>& candidates,
                                     VkImageTiling tiling,
                                     VkFormatFeatureFlags features ) const
    -> VkFormat {
    for ( VkFormat format : candidates ) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties( pDevice_, format, &props );

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

ContextQt::~ContextQt() {
}

}  // namespace tire::vk