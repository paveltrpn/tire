

#include "vulkan/vulkan.h"
#include <vulkan/vk_enum_string_helper.h>

#include "config/config.h"
#include "context_qt.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_CONTEXT_CPP{ true };

namespace tire::vk {

ContextQt::ContextQt( VkInstance instance, VkPhysicalDevice pDevice,
                      VkDevice device, VkSurfaceKHR surface, VkRenderPass rp,
                      uint32_t gqfi, uint32_t gqi )
    : graphicsQueueId_{ gqi } {
    instance_ = instance;
    physDevice_ = pDevice;
    device_ = device;
    surface_ = surface;
    renderPass_ = rp;
    graphicsFamilyQueueId_ = gqfi;
}

auto ContextQt::init() -> void {
    uint32_t version{};
    vkEnumerateInstanceVersion( &version );
    log::info( "vk::Context === acquired api instance version is {}.{}",
               VK_API_VERSION_MAJOR( version ),
               VK_API_VERSION_MINOR( version ) );

    queryDeviceInfo();
    querySurface();

    // NOTE: ackquire viewport size from surface!!!
    const auto configHandle = tire::Config::instance();
    const auto width = configHandle->get<int>( "window_width" );
    const auto height = configHandle->get<int>( "window_height" );
    setViewportSize( width, height );

    std::cout << surfaceCapabilities_.currentExtent.height
              << " ================\n";
    // Graphic queue.
    vkGetDeviceQueue( device_, graphicsFamilyQueueId_, graphicsQueueId_,
                      &graphicsQueue_ );
}

auto ContextQt::queryDeviceInfo() -> void {
    // Collect physical devices and its properties
    vkGetPhysicalDeviceProperties( physDevice_, &pDeviceProperties_ );
    log::info( "vk::Device === name: {}", pDeviceProperties_.deviceName );

    vkGetPhysicalDeviceFeatures( physDevice_, &pDeviceFeatures_ );

    // Collect physical device queue families properies
    uint32_t queueFamilyCount{};
    vkGetPhysicalDeviceQueueFamilyProperties( physDevice_, &queueFamilyCount,
                                              nullptr );

    queueFamilyProperties_.resize( queueFamilyCount );
    vkGetPhysicalDeviceQueueFamilyProperties( physDevice_, &queueFamilyCount,
                                              queueFamilyProperties_.data() );
    log::debug<DEBUG_OUTPUT_CONTEXT_CPP>(
        "vk::Device === device queue family count: {}", queueFamilyCount );

    // Collect physical device extensions info
    uint32_t extensionCount{};
    if ( const auto err = vkEnumerateDeviceExtensionProperties(
             physDevice_, nullptr, &extensionCount, nullptr );
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
             physDevice_, nullptr, &extensionCount, pDeviceExtensions_.data() );
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
             physDevice_, surface_, &surfaceCapabilities_ );
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

ContextQt::~ContextQt() {
}

}  // namespace tire::vk