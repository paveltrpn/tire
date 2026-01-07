
module;

#include <set>

#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vk_enum_string_helper.h>

export module context:device;

import config;
import log;

import :context;

namespace tire {

void Context::collectPhysicalDevices() {
    uint32_t devCount{};

    // Enumerate physical devices
    if ( const auto err = vkEnumeratePhysicalDevices( instance_, &devCount, nullptr ); err != VK_SUCCESS ) {
        log::fatal( "can't enumerate physical devices with code: \n", string_VkResult( err ) );
    } else {
        log::debug()( "physical devices enumerate success, count: {}", devCount );
    }

    if ( devCount == 0 ) {
        log::fatal( "no vulkan physical devices in system\n" );
    }

    std::vector<VkPhysicalDevice> physicalDevices{};
    physicalDevices.resize( devCount );

    // Get physical devices
    if ( const auto err = vkEnumeratePhysicalDevices( instance_, &devCount, physicalDevices.data() );
         err != VK_SUCCESS ) {
        log::fatal( "can't acquire physical devices with code: {}\n", string_VkResult( err ) );
    } else {
        log::debug()( "physical devices acquire success" );
    }

    // Get physical device info for each device
    log::info()( "collect physical device info..." );
    for ( const auto device : physicalDevices ) {
        // Collect physical devices and its properties

        VkPhysicalDeviceProperties devProps;
        vkGetPhysicalDeviceProperties( device, &devProps );
        log::info()( "device name: {}", devProps.deviceName );

        VkPhysicalDeviceFeatures devFeatures;
        vkGetPhysicalDeviceFeatures( device, &devFeatures );

        // Collect physical device queue families properies
        uint32_t queueFamilyCount{};
        vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount, nullptr );

        std::vector<VkQueueFamilyProperties> qfp{};
        qfp.resize( queueFamilyCount );
        vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount, qfp.data() );
        log::debug()( "device queue family count: {}", queueFamilyCount );

        // Collect physical device extensions info
        uint32_t extensionCount{};
        if ( const auto err = vkEnumerateDeviceExtensionProperties( device, nullptr, &extensionCount, nullptr );
             err != VK_SUCCESS ) {
            log::fatal(
              "can't enumerate physical device extensions "
              "with code: {}\n",
              string_VkResult( err ) );
        } else {
            log::debug()(
              "physical device extensions enumerated for "
              "device: {}, "
              "count: {}",
              devProps.deviceName, extensionCount );
        }

        std::vector<VkExtensionProperties> availableExtensions;
        availableExtensions.resize( extensionCount );

        if ( const auto err =
               vkEnumerateDeviceExtensionProperties( device, nullptr, &extensionCount, availableExtensions.data() );
             err != VK_SUCCESS ) {
            log::fatal(
              "can't acquire physical device extensions "
              "with code: {}\n",
              string_VkResult( err ) );
        } else {
            log::debug()(
              "physical device extensions acquired for "
              "device: "
              "{}",
              devProps.deviceName );
        }

        // Store physical device in list
        physicalDevices_.emplace_back(
          PhysicalDevice{
            .device = device,
            .properties = devProps,
            .features = devFeatures,
            .extensions = availableExtensions,
            .queueFamilyProperties = qfp } );
    }
}

void Context::makeDevice() {
    const auto configHandle = Config::instance();

    // Check which devices available on machine
    int discreetGpuId{ -1 };
    int integratedGpuId{ -1 };
    int otherGpuId{ -1 };
    int virtualGpuId{ -1 };
    int cpuGpuId{ -1 };
    for ( auto i{ 0 }; const auto &physicalDevice : physicalDevices_ ) {
        const auto &deviceProps = physicalDevice.properties;
        switch ( physicalDevice.properties.deviceType ) {
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: {
                discreetGpuId = i;
                break;
            }
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: {
                integratedGpuId = i;
                break;
            }
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: {
                virtualGpuId = i;
                break;
            }
            case VK_PHYSICAL_DEVICE_TYPE_CPU: {
                cpuGpuId = i;
                break;
            }
            case VK_PHYSICAL_DEVICE_TYPE_OTHER: {
                otherGpuId = i;
                break;
            }
            default: {
                break;
            }
        }
        ++i;
    }

    // First try to pick discreete GPU
    if ( discreetGpuId != -1 ) {
        pickedPhysicalDeviceId_ = discreetGpuId;
    } else if ( integratedGpuId != -1 ) {
        pickedPhysicalDeviceId_ = integratedGpuId;
    } else if ( cpuGpuId != -1 ) {
        pickedPhysicalDeviceId_ = cpuGpuId;
    } else {
        log::fatal( "vk::Device === no suitable vulkan devices found! " );
    }

    // Base class member.
    physDevice_ = physicalDevices_[pickedPhysicalDeviceId_].device;

    log::info()( "pick {}", physicalDevices_[pickedPhysicalDeviceId_].properties.deviceName );

    // Choose queue family with VK_QUEUE_GRAPHICS_BIT.
    // The good news is that
    // any queue family with VK_QUEUE_GRAPHICS_BIT or VK_QUEUE_COMPUTE_BIT ca-
    // pabilities already implicitly support VK_QUEUE_TRANSFER_BIT operations.
    for ( auto i{ 0 }; const auto &queueFamily : physicalDevices_[pickedPhysicalDeviceId_].queueFamilyProperties ) {
        if ( queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT ) {
            graphicsFamilyQueueId_ = i;
            break;
        }
        ++i;
    }

    if ( graphicsFamilyQueueId_ == UINT32_MAX ) {
        log::fatal(
          "failed to get device with queue family that "
          "VK_QUEUE_GRAPHICS_BIT" );
    }

    // Check if picked queue family (with VK_QUEUE_GRAPHICS_BIT) support present.
    // May exist other queue family that not support VK_QUEUE_GRAPHICS_BIT but support
    // present and we can use it to present, but now we just check "can present" property
    // on coosed one and if not we terminate.
    VkBool32 presentSupport = false;
    if ( const auto err = vkGetPhysicalDeviceSurfaceSupportKHR(
           physicalDevices_[pickedPhysicalDeviceId_].device, graphicsFamilyQueueId_, surface_, &presentSupport );
         err != VK_SUCCESS ) {
        log::fatal(
          "failed to get device surface support for "
          "presentation with code {}!\n",
          string_VkResult( err ) );
    }

    if ( presentSupport ) {
        presentSupportQueueId_ = graphicsFamilyQueueId_;
    } else {
        log::fatal(
          "queue with VK_QUEUE_GRAPHICS_BIT not support present! Maybe check "
          "another queue family?" );
    }

    log::debug()( "graphics family: {}", graphicsFamilyQueueId_ );
    log::debug()( "present family: {}", presentSupportQueueId_ );

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    const std::set<uint32_t> uniqueQueueFamilies = { graphicsFamilyQueueId_, presentSupportQueueId_ };

    const float queuePriority{ 1.0f };
    for ( const uint32_t queueFamily : uniqueQueueFamilies ) {
        VkDeviceQueueCreateInfo queueCreateInfo{
          .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
          .queueFamilyIndex = queueFamily,
          .queueCount = 1,
          .pQueuePriorities = &queuePriority };
        queueCreateInfos.push_back( queueCreateInfo );
    }

    std::vector<const char *> desiredExtensionsList{};
    desiredExtensionsList.emplace_back( "VK_KHR_swapchain" );

    if ( configHandle->get<bool>( "enable_raytracing_extensions" ) ) {
        log::info()( "raytracing extansions enabled" );

        desiredExtensionsList.emplace_back( "VK_KHR_ray_query" );
        desiredExtensionsList.emplace_back( "VK_KHR_ray_tracing_pipeline" );
        desiredExtensionsList.emplace_back( "VK_KHR_ray_tracing_maintenance1" );
        desiredExtensionsList.emplace_back( "VK_KHR_ray_tracing_position_fetch" );
        desiredExtensionsList.emplace_back( "VK_KHR_acceleration_structure" );
        desiredExtensionsList.emplace_back( "VK_EXT_descriptor_indexing" );
        desiredExtensionsList.emplace_back( "VK_KHR_buffer_device_address" );
        desiredExtensionsList.emplace_back( "VK_KHR_deferred_host_operations" );
        desiredExtensionsList.emplace_back( "VK_KHR_spirv_1_4" );
        desiredExtensionsList.emplace_back( "VK_KHR_shader_float_controls" );
    } else {
        log::info()( "raytracing extansions disabled" );
    }

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>( queueCreateInfos.size() );
    deviceCreateInfo.pEnabledFeatures = &physicalDevices_[pickedPhysicalDeviceId_].features;
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>( desiredExtensionsList.size() );
    deviceCreateInfo.ppEnabledExtensionNames = desiredExtensionsList.data();

    // NOTE: cannot use "import config" as c++ module bcause of clang 20 bug - "error: 'lifetimebound' attribute
    // cannot be applied to a parameter of a function that returns void; did you mean 'lifetime_capture_by(X)'"
    //
    // Force use validation layers

    if ( configHandle->get<bool>( "enable_validation_layers" ) ) {
        deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>( desiredValidationLayerList_.size() );
        deviceCreateInfo.ppEnabledLayerNames = desiredValidationLayerList_.data();
    } else {
        deviceCreateInfo.enabledLayerCount = 0;
        deviceCreateInfo.ppEnabledLayerNames = nullptr;
    }

    // Create a logical device
    if ( const auto err = vkCreateDevice( physDevice_, &deviceCreateInfo, nullptr, &device_ ); err != VK_SUCCESS ) {
        log::fatal( "failed to create logical device with code {}!\n", string_VkResult( err ) );
    } else {
        log::info()( "logical device create success!" );
    }

    // Graphic and present queue id
    vkGetDeviceQueue( device_, graphicsFamilyQueueId_, 0, &graphicsQueue_ );
    vkGetDeviceQueue( device_, presentSupportQueueId_, 0, &presentQueue_ );

    // Physical device surface capabilities
    if ( const auto err = vkGetPhysicalDeviceSurfaceCapabilitiesKHR( physDevice_, surface_, &surfaceCapabilities_ );
         err != VK_SUCCESS ) {
        log::fatal( "failed to obtain surface capabilities with code {}!\n", string_VkResult( err ) );
    } else {
        log::debug()( "physical device surface capabilities acquire "
                      "success!" );
    }

    // Application window size (extent) givet at startup.
    // NOTE: What i have to do with this when application window
    // have ben resized??
    currentExtent_ = surfaceCapabilities_.currentExtent;

    // Physical device surface formats
    uint32_t formatCount{};
    if ( const auto err = vkGetPhysicalDeviceSurfaceFormatsKHR( physDevice_, surface_, &formatCount, nullptr );
         err != VK_SUCCESS ) {
        log::fatal(
          "failed to obtain physical device surface formats "
          "count with code {}!\n",
          string_VkResult( err ) );
    } else {
        log::debug()(
          "physical device surface formats count obtain "
          "success, "
          "count: "
          "{}",
          formatCount );
    }

    surfaceFormats_.resize( formatCount );

    if ( const auto err =
           vkGetPhysicalDeviceSurfaceFormatsKHR( physDevice_, surface_, &formatCount, surfaceFormats_.data() );
         err != VK_SUCCESS ) {
        log::fatal(
          "failed to acquire physical device surface formats "
          "count with code {}!\n",
          string_VkResult( err ) );
    } else {
        log::debug()( "physical device surface formats acquire "
                      "success!" );
    }

    for ( const auto &item : surfaceFormats_ ) {
        log::notice()( "format: {}", string_VkFormat( item.format ) );
    }

    // Physical device present modes
    uint32_t presentModeCount{};
    if ( const auto err =
           vkGetPhysicalDeviceSurfacePresentModesKHR( physDevice_, surface_, &presentModeCount, nullptr );
         err != VK_SUCCESS ) {
        log::fatal(
          "failed to obtain physical device present modes "
          "count with code {}!\n",
          string_VkResult( err ) );
    } else {
        log::debug()(
          "physical device present modes count obtain "
          "success, "
          "count: "
          "{}",
          presentModeCount );
    }

    presentModes_.resize( presentModeCount );

    if ( const auto err =
           vkGetPhysicalDeviceSurfacePresentModesKHR( physDevice_, surface_, &presentModeCount, presentModes_.data() );
         err != VK_SUCCESS ) {
        log::fatal(
          "failed to acquire physical device present modes "
          "with code {}!\n",
          string_VkResult( err ) );
    } else {
        log::debug()( "physical device present modes acquire success!" );
    }

    for ( const auto &item : presentModes_ ) {
        log::notice()( "mode: {}", string_VkPresentModeKHR( item ) );
    }

#define CHOSEN_SURFACE_FORMAT 0
    // This format will be used across application, in
    // swapchain in particular.
    surfaceFormat_ = surfaceFormats_[CHOSEN_SURFACE_FORMAT];

    log::debug()( "format is {}", string_VkFormat( surfaceFormat_.format ) );

#define CHOSEN_PRESENT_MODE VK_PRESENT_MODE_FIFO_KHR
    // This present mode will be used across application, in
    // swapchain in particular.
    presentMode_ = CHOSEN_PRESENT_MODE;  // presentModes_[0];

    log::debug()( "present mode is {}", string_VkPresentModeKHR( presentMode_ ) );
}

}  // namespace tire
