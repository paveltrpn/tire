
#include <set>
#include <vulkan/vk_enum_string_helper.h>

#include "device.h"
#include "config/config.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_DEVICEVK_CPP{ true };

namespace tire::vk {

Device::Device( const vk::Instance *instance, const vk::Surface *surface )
    : instance_{ instance }
    , surface_{ surface } {
    uint32_t devCount{};

    if ( const auto err = vkEnumeratePhysicalDevices( instance_->handle(),
                                                      &devCount, nullptr );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "can't enumerate physical devices with code: {}\n",
                         string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
            "vk::Device === physical devices enumerate success, count: {}",
            devCount );
    }

    if ( devCount == 0 ) {
        throw std::runtime_error( "no vulkan physical devices in system\n" );
    }

    std::vector<VkPhysicalDevice> physicalDevices( devCount );

    if ( const auto err = vkEnumeratePhysicalDevices(
             instance_->handle(), &devCount, physicalDevices.data() );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "can't acquire physical devices with code: {}\n",
                         string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
            "vk::Device === physical devices acquire success" );
    }

    log::info( "vk::Device === collect physical device info..." );
    for ( const auto device : physicalDevices ) {
        // Collect physical devices and its properties

        VkPhysicalDeviceProperties devProps;
        vkGetPhysicalDeviceProperties( device, &devProps );
        log::info( "vk::Device === name: {}", devProps.deviceName );

        VkPhysicalDeviceFeatures devFeatures;
        vkGetPhysicalDeviceFeatures( device, &devFeatures );

        // Collect physical device queue families properies
        uint32_t queueFamilyCount{};
        vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount,
                                                  nullptr );
        std::vector<VkQueueFamilyProperties> qfp( queueFamilyCount );
        vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount,
                                                  qfp.data() );
        log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
            "vk::Device === device queue family count: {}", queueFamilyCount );

        uint32_t extensionCount{};
        if ( const auto err = vkEnumerateDeviceExtensionProperties(
                 device, nullptr, &extensionCount, nullptr );
             err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "can't enumerate physical device extensions "
                             "with code: {}\n",
                             string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
                "vk::Device === physical device extensions enumerated for "
                "device: {}, "
                "count: {}",
                devProps.deviceName, extensionCount );
        }

        std::vector<VkExtensionProperties> availableExtensions(
            extensionCount );

        if ( const auto err = vkEnumerateDeviceExtensionProperties(
                 device, nullptr, &extensionCount, availableExtensions.data() );
             err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "can't acquire physical device extensions "
                             "with code: {}\n",
                             string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
                "vk::Device === physical device extensions acquired for "
                "device: "
                "{}",
                devProps.deviceName );
        }

        physicalDevices_.emplace_back(
            PhysicalDevice{ .device = device,
                            .properties = devProps,
                            .features = devFeatures,
                            .extensions = availableExtensions,
                            .queueFamilyProperties = qfp } );
    }
}

Device::~Device() {
    vkDestroyDevice( device_, nullptr );
}

VkDevice Device::handle() const {
    return device_;
}

// Create a new device instance. A logical device is created as a connection to a physical device.
// Check is physical device suitable, can be done acoording to
// physical devices properties and physical device queue families properies
void Device::pickAndCreateDevice() {
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
    int pickedPhysicalDeviceId{ -1 };
    if ( discreetGpuId != -1 ) {
        pickedPhysicalDeviceId = discreetGpuId;
    } else if ( integratedGpuId != -1 ) {
        pickedPhysicalDeviceId = integratedGpuId;
    } else if ( cpuGpuId != -1 ) {
        pickedPhysicalDeviceId = cpuGpuId;
    } else {
        throw std::runtime_error( "no suitable vulkan devices found! " );
    }

    log::info( "vk::Device === pick {}",
               physicalDevices_[pickedPhysicalDeviceId].properties.deviceName );

    // Choose queue family with VK_QUEUE_GRAPHICS_BIT
    for ( auto i{ 0 };
          const auto &queueFamily :
          physicalDevices_[pickedPhysicalDeviceId].queueFamilyProperties ) {
        if ( queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT ) {
            graphicsFamilyQueueId_ = i;
            break;
        }
        ++i;
    }

    if ( graphicsFamilyQueueId_ == UINT_MAX ) {
        throw std::runtime_error(
            "failed to get device with queue family that "
            "VK_QUEUE_GRAPHICS_BIT" );
    }

    // Check if picked queue family (with VK_QUEUE_GRAPHICS_BIT) support present.
    // May exist other queue family that not support VK_QUEUE_GRAPHICS_BIT but support
    // present and we can use it to present, but now we just check "can present" property
    // on coosed one and if not we terminate.
    VkBool32 presentSupport = false;
    if ( const auto err = vkGetPhysicalDeviceSurfaceSupportKHR(
             physicalDevices_[pickedPhysicalDeviceId].device,
             graphicsFamilyQueueId_, surface_->handle(), &presentSupport );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to get device surface support for "
                         "presentation with code {}!\n",
                         string_VkResult( err ) ) );
    }

    if ( presentSupport ) {
        presentSupportQueueId_ = graphicsFamilyQueueId_;
    } else {
        throw std::runtime_error(
            "queue with VK_QUEUE_GRAPHICS_BIT not support present! Maybe check "
            "another queue family?" );
    }

    log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>( "vk::Device === graphics family: {}",
                                           graphicsFamilyQueueId_ );
    log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>( "vk::Device === present family: {}",
                                           presentSupportQueueId_ );

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    const std::set<uint32_t> uniqueQueueFamilies = { graphicsFamilyQueueId_,
                                                     presentSupportQueueId_ };

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
    desiredExtensionsList.emplace_back( "VK_KHR_ray_query" );
    desiredExtensionsList.emplace_back( "VK_KHR_ray_tracing_pipeline" );
    desiredExtensionsList.emplace_back( "VK_KHR_ray_tracing_maintenance1" );
    desiredExtensionsList.emplace_back( "VK_KHR_ray_tracing_position_fetch" );
    desiredExtensionsList.emplace_back( "VK_KHR_acceleration_structure" );
    desiredExtensionsList.emplace_back( "VK_EXT_descriptor_indexing" );
    desiredExtensionsList.emplace_back( "VK_KHR_maintenance3" );
    desiredExtensionsList.emplace_back( "VK_KHR_buffer_device_address" );
    desiredExtensionsList.emplace_back( "VK_KHR_deferred_host_operations" );

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.queueCreateInfoCount =
        static_cast<uint32_t>( queueCreateInfos.size() );
    deviceCreateInfo.pEnabledFeatures =
        &physicalDevices_[pickedPhysicalDeviceId].features;
    deviceCreateInfo.enabledExtensionCount =
        static_cast<uint32_t>( desiredExtensionsList.size() );
    deviceCreateInfo.ppEnabledExtensionNames = desiredExtensionsList.data();

    if ( Config::instance()->get<bool>( "enable_validation_layers" ) ) {
        const auto [size, data] = instance_->validationLayers();
        deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>( size );
        deviceCreateInfo.ppEnabledLayerNames = data;
    } else {
        deviceCreateInfo.enabledLayerCount = 0;
    }

    // Create a logical device
    if ( const auto err =
             vkCreateDevice( physicalDevices_[pickedPhysicalDeviceId].device,
                             &deviceCreateInfo, nullptr, &device_ );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create logical device with code {}!\n",
                         string_VkResult( err ) ) );
    } else {
        log::info( "vk::Device === logical device create success!" );
    }

    vkGetDeviceQueue( device_, graphicsFamilyQueueId_, 0, &graphicsQueue_ );
    vkGetDeviceQueue( device_, presentSupportQueueId_, 0, &presentQueue_ );

    // physical device surface capabilities

    if ( const auto err = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
             physicalDevices_[pickedPhysicalDeviceId].device,
             surface_->handle(), &surfaceCapabilities_ );
         err != VK_SUCCESS ) {
        throw std::runtime_error( std::format(
            "failed to obtain surface capabilities with code {}!\n",
            string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
            "vk::Device === physical device surface capabilities acquire "
            "success!" );
    }

    // Application window size (extent) givet at startup.
    // NOTE: What i have to do with this when application window
    // have ben resized??
    currentExtent_ = surfaceCapabilities_.currentExtent;

    // physical device surface formats
    uint32_t formatCount;
    if ( const auto err = vkGetPhysicalDeviceSurfaceFormatsKHR(
             physicalDevices_[pickedPhysicalDeviceId].device,
             surface_->handle(), &formatCount, nullptr );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to obtain physical device surface formats "
                         "count with code {}!\n",
                         string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
            "vk::Device === physical device surface formats count obtain "
            "success, "
            "count: "
            "{}",
            formatCount );
    }

    surfaceFormats_.resize( formatCount );

    if ( const auto err = vkGetPhysicalDeviceSurfaceFormatsKHR(
             physicalDevices_[pickedPhysicalDeviceId].device,
             surface_->handle(), &formatCount, surfaceFormats_.data() );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to acquire physical device surface formats "
                         "count with code {}!\n",
                         string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
            "vk::Device === physical device surface formats acquire "
            "success!" );
    }

// This format will be used across application, in
// swapchain in particular.
// NOTE: Make choose liitle wiser in future
#define CHOSEN_SURFACE_FORMAT 0
    surfaceFormat_ = surfaceFormats_[CHOSEN_SURFACE_FORMAT];

    log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
        "vk::Device === device format is {}",
        string_VkFormat( surfaceFormat_.format ) );

    // physical device present modes
    uint32_t presentModeCount;
    if ( const auto err = vkGetPhysicalDeviceSurfacePresentModesKHR(
             physicalDevices_[pickedPhysicalDeviceId].device,
             surface_->handle(), &presentModeCount, nullptr );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to obtain physical device present modes "
                         "count with code {}!\n",
                         string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
            "vk::Device === physical device present modes count obtain "
            "success, "
            "count: "
            "{}",
            presentModeCount );
    }

    presentModes_.resize( presentModeCount );

    if ( const auto err = vkGetPhysicalDeviceSurfacePresentModesKHR(
             physicalDevices_[pickedPhysicalDeviceId].device,
             surface_->handle(), &presentModeCount, presentModes_.data() );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to acquire physical device present modes "
                         "with code {}!\n",
                         string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
            "vk::Device === physical device present modes acquire success!" );
    }

// This present mode will be used across application, in
// swapchain in particular.
// NOTE: Make choose liitle wiser in future
#define CHOSEN_PRESENT_MODE 0
    presentMode_ = presentModes_[CHOSEN_PRESENT_MODE];
}

void Device::displayRenderInfo() {
    for ( size_t i = 0; i < physicalDevices_.size(); ++i ) {
        displayPhysicalDeviceProperties( i );
        displayPhysicalDeviceFeatures( i );
        displayPhysicalDeviceFamiliesProperties( i );
    }
}

void Device::displayPhysicalDeviceProperties( size_t id ) {
    std::print(
        R"(physical device properties:
================
)" );

    auto &d = physicalDevices_[id].properties;
    std::print(
        R"(	deviceName:	{}
    deviceID:	{}
    vendorID:	{}
    apiVersion:	{}
    deviceType: not printed
)",
        d.deviceName, d.deviceID, d.vendorID, d.apiVersion );
}

void Device::displayPhysicalDeviceFeatures( size_t id ) {
    std::print(
        R"(physical device features:
================
)" );

    auto &d = physicalDevices_[id].features;
    std::print(
        R"(	robustBufferAccess:	{}
    fullDrawIndexUint32:	{}
    imageCubeArray:	{}
    independentBlend:	{}
    geometryShader:	{}
    tessellationShader:	{}
    sampleRateShading:	{}
    dualSrcBlend:	{}
    logicOp:	{}
    multiDrawIndirect:	{}
    drawIndirectFirstInstance:	{}
    depthClamp:	{}
    depthBiasClamp:	{}
    fillModeNonSolid:	{}
    depthBounds:	{}
    wideLines:	{}
    largePoints:	{}
    alphaToOne:	{}
    multiViewport:	{}
    samplerAnisotropy:	{}
    textureCompressionETC2:	{}
    textureCompressionASTC_LDR:	{}
    textureCompressionBC:	{}
    occlusionQueryPrecise:	{}
    pipelineStatisticsQuery:	{}
    vertexPipelineStoresAndAtomics:	{}
    fragmentStoresAndAtomics:	{}
    shaderTessellationAndGeometryPointSize:	{}
    shaderImageGatherExtended:	{}
    shaderStorageImageExtendedFormats:	{}
    shaderStorageImageMultisample:	{}
    shaderStorageImageReadWithoutFormat:	{}
    shaderStorageImageWriteWithoutFormat:	{}
    shaderUniformBufferArrayDynamicIndexing:	{}
    shaderSampledImageArrayDynamicIndexing:	{}
    shaderStorageBufferArrayDynamicIndexing:	{}
    shaderStorageImageArrayDynamicIndexing:	{}
    shaderClipDistance:	{}
    shaderCullDistance:	{}
    shaderFloat64:	{}
    shaderInt64:	{}
    shaderInt16:	{}
    shaderResourceResidency:	{}
    shaderResourceMinLod:	{}
    sparseBinding:	{}
    sparseResidencyBuffer:	{}
    sparseResidencyImage2D:	{}
    sparseResidencyImage3D:	{}
    sparseResidency2Samples:	{}
    sparseResidency4Samples:	{}
    sparseResidency8Samples:	{}
    sparseResidency16Samples:	{}
    sparseResidencyAliased:	{}
    variableMultisampleRate:	{}
    inheritedQueries:	{}
)",
        d.robustBufferAccess, d.fullDrawIndexUint32, d.imageCubeArray,
        d.independentBlend, d.geometryShader, d.tessellationShader,
        d.sampleRateShading, d.dualSrcBlend, d.logicOp, d.multiDrawIndirect,
        d.drawIndirectFirstInstance, d.depthClamp, d.depthBiasClamp,
        d.fillModeNonSolid, d.depthBounds, d.wideLines, d.largePoints,
        d.alphaToOne, d.multiViewport, d.samplerAnisotropy,
        d.textureCompressionETC2, d.textureCompressionASTC_LDR,
        d.textureCompressionBC, d.occlusionQueryPrecise,
        d.pipelineStatisticsQuery, d.vertexPipelineStoresAndAtomics,
        d.fragmentStoresAndAtomics, d.shaderTessellationAndGeometryPointSize,
        d.shaderImageGatherExtended, d.shaderStorageImageExtendedFormats,
        d.shaderStorageImageMultisample, d.shaderStorageImageReadWithoutFormat,
        d.shaderStorageImageWriteWithoutFormat,
        d.shaderUniformBufferArrayDynamicIndexing,
        d.shaderSampledImageArrayDynamicIndexing,
        d.shaderStorageBufferArrayDynamicIndexing,
        d.shaderStorageImageArrayDynamicIndexing, d.shaderClipDistance,
        d.shaderCullDistance, d.shaderFloat64, d.shaderInt64, d.shaderInt16,
        d.shaderResourceResidency, d.shaderResourceMinLod, d.sparseBinding,
        d.sparseResidencyBuffer, d.sparseResidencyImage2D,
        d.sparseResidencyImage3D, d.sparseResidency2Samples,
        d.sparseResidency4Samples, d.sparseResidency8Samples,
        d.sparseResidency16Samples, d.sparseResidencyAliased,
        d.variableMultisampleRate, d.inheritedQueries );
}

void Device::displayPhysicalDeviceFamiliesProperties( size_t id ) {
    std::print(
        R"(physical device families properties:
================
)" );

    const auto &queueFamilyProperties =
        physicalDevices_[id].queueFamilyProperties;

    for ( const auto &prop : queueFamilyProperties ) {
        std::print(
            R"(	queueFlags:	{}
    queueCount:	{}
    timestampValidBits:	{}
    minImageTransferGranularity: not printed
)",
            prop.queueFlags, prop.queueCount, prop.timestampValidBits );
    }
}

void Device::displaySurfaceCapabilities() {
    std::print(
        R"(physical device surface capabilities:
================
)" );

    std::print(
        R"(	minImageCount:		{}
        maxImageCount:		{}
        maxImageArrayLayers:	{}
        currentExtent.width:	{}
        currentExtent.height:	{}
)",
        surfaceCapabilities_.minImageCount, surfaceCapabilities_.maxImageCount,
        surfaceCapabilities_.maxImageArrayLayers,
        surfaceCapabilities_.currentExtent.width,
        surfaceCapabilities_.currentExtent.height );
}

}  // namespace tire::vk
