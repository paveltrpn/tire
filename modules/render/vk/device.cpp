
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

    {
        const auto err = vkEnumeratePhysicalDevices( instance_->handle(),
                                                     &devCount, nullptr );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "can't enumerate physical devices with code: {}\n",
                             string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
                "physical devices enumerate success, count: {}", devCount );
        }
    }

    if ( devCount == 0 ) {
        throw std::runtime_error( "no vulkan physical devices in system\n" );
    }

    std::vector<VkPhysicalDevice> physicalDevices( devCount );

    {
        const auto err = vkEnumeratePhysicalDevices(
            instance_->handle(), &devCount, physicalDevices.data() );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "can't acquire physical devices with code: {}\n",
                             string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
                "physical devices acquire success" );
        }
    }

    log::info( " === collect physical device info..." );
    for ( const auto device : physicalDevices ) {
        // Collect physical devices and its properties

        VkPhysicalDeviceProperties devProps;
        vkGetPhysicalDeviceProperties( device, &devProps );
        log::info( "device name: {}", devProps.deviceName );

        VkPhysicalDeviceFeatures devFeatures;
        vkGetPhysicalDeviceFeatures( device, &devFeatures );

        // Collect physical device queue families properies
        uint32_t queueFamilyCount{};
        vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount,
                                                  nullptr );
        std::vector<VkQueueFamilyProperties> qfp( queueFamilyCount );
        vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount,
                                                  qfp.data() );
        log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>( "device queue family count: {}",
                                               queueFamilyCount );

        uint32_t extensionCount{};
        {
            const auto err = vkEnumerateDeviceExtensionProperties(
                device, nullptr, &extensionCount, nullptr );
            if ( err != VK_SUCCESS ) {
                throw std::runtime_error(
                    std::format( "can't enumerate physical device extensions "
                                 "with code: {}\n",
                                 string_VkResult( err ) ) );
            } else {
                log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
                    "physical device extensions enumerated for device: {}, "
                    "count: {}",
                    devProps.deviceName, extensionCount );
            }
        }

        std::vector<VkExtensionProperties> availableExtensions(
            extensionCount );

        {
            const auto err = vkEnumerateDeviceExtensionProperties(
                device, nullptr, &extensionCount, availableExtensions.data() );
            if ( err != VK_SUCCESS ) {
                throw std::runtime_error(
                    std::format( "can't acquire physical device extensions "
                                 "with code: {}\n",
                                 string_VkResult( err ) ) );
            } else {
                log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
                    "physical device extensions acquired for device: {}",
                    devProps.deviceName );
            }
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

void Device::pickAndCreateDevice( size_t id ) {
    // Create a new device instance. A logical device is created as a connection to a physical device.
    // Check is physical device suitable, can be done acoording to
    // physical devices properties and physical device queue families properies

    log::info( "pick device: {}", physicalDevices_[id].properties.deviceName );

    // Condition: we pick discreete GPU
    const auto &deviceProps = physicalDevices_[id].properties;
    if ( !( deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ) ) {
        throw std::runtime_error(
            "picked physical device is not discrete GPU!" );
    }

    int i{};
    for ( const auto &queueFamily :
          physicalDevices_[id].queueFamilyProperties ) {
        // Condition: we need queue with VK_QUEUE_GRAPHICS_BIT
        if ( queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT ) {
            graphicsFamily_ = i;
        }
        VkBool32 presentSupport = false;
        // Condition: we need device that support surface presentation
        const auto err = vkGetPhysicalDeviceSurfaceSupportKHR(
            physicalDevices_[id].device, i, surface_->handle(),
            &presentSupport );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to get device surface support for "
                             "presentation with code {}!\n",
                             string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
                "device surface support for "
                "presentation acquire success!" );
        }

        if ( presentSupport ) {
            presentFamily_ = i;
        }
        i++;
    }

    log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>( "graphics family: {}",
                                           graphicsFamily_ );
    log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>( "present family: {}",
                                           presentFamily_ );

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    const std::set<uint32_t> uniqueQueueFamilies = { graphicsFamily_,
                                                     presentFamily_ };

    const float queuePriority{ 1.0f };
    for ( const uint32_t queueFamily : uniqueQueueFamilies ) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back( queueCreateInfo );
    }

    VkDeviceCreateInfo deviceCreateInfo{};

    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.queueCreateInfoCount =
        static_cast<uint32_t>( queueCreateInfos.size() );
    deviceCreateInfo.pEnabledFeatures = &physicalDevices_[id].features;

    // Condition: we need device that support swapchain extension
    std::vector<const char *> elist = { "VK_KHR_swapchain" };
    deviceCreateInfo.enabledExtensionCount =
        static_cast<uint32_t>( elist.size() );
    deviceCreateInfo.ppEnabledExtensionNames = elist.data();

    if ( Config::instance()->get<bool>( "enable_validation_layers" ) ) {
        const auto [size, data] = instance_->validationLayersInfo();
        deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>( size );
        deviceCreateInfo.ppEnabledLayerNames = data;
    } else {
        deviceCreateInfo.enabledLayerCount = 0;
    }

    // Create a logical device
    {
        const auto err = vkCreateDevice( physicalDevices_[id].device,
                                         &deviceCreateInfo, nullptr, &device_ );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to create logical device with code {}!\n",
                             string_VkResult( err ) ) );
        } else {
            log::info( "logical device create success!" );
        }
    }

    vkGetDeviceQueue( device_, graphicsFamily_, 0, &graphicsQueue_ );
    vkGetDeviceQueue( device_, presentFamily_, 0, &presentQueue_ );

    // physical device surface capabilities
    {
        const auto err = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            physicalDevices_[id].device, surface_->handle(),
            &surfaceCapabilities_ );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error( std::format(
                "failed to obtain surface capabilities with code {}!\n",
                string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
                "physical device surface capabilities acquire success!" );
        }
    }

    // physical device surface formats
    uint32_t formatCount;
    {
        const auto err = vkGetPhysicalDeviceSurfaceFormatsKHR(
            physicalDevices_[id].device, surface_->handle(), &formatCount,
            nullptr );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to obtain physical device surface formats "
                             "count with code {}!\n",
                             string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
                "physical device surface formats count obtain success, "
                "count: "
                "{}",
                formatCount );
        }
    }

    surfaceFormats_.resize( formatCount );

    {
        const auto err = vkGetPhysicalDeviceSurfaceFormatsKHR(
            physicalDevices_[id].device, surface_->handle(), &formatCount,
            surfaceFormats_.data() );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error( std::format(
                "failed to acquire physical device surface formats "
                "count with code {}!\n",
                string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
                "physical device surface formats acquire success!" );
        }
    }

    // This format will be used across application, in
    // swapchain in particular. Make choose liitle wiser in future
#define CHOSEN_SURFACE_FORMAT 0
    surfaceFormat_ = surfaceFormats_[CHOSEN_SURFACE_FORMAT];

    // physical device present modes
    uint32_t presentModeCount;
    {
        const auto err = vkGetPhysicalDeviceSurfacePresentModesKHR(
            physicalDevices_[id].device, surface_->handle(), &presentModeCount,
            nullptr );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to obtain physical device present modes "
                             "count with code {}!\n",
                             string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
                "physical device present modes count obtain success, "
                "count: "
                "{}",
                presentModeCount );
        }
    }

    presentModes_.resize( presentModeCount );

    {
        const auto err = vkGetPhysicalDeviceSurfacePresentModesKHR(
            physicalDevices_[id].device, surface_->handle(), &presentModeCount,
            presentModes_.data() );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to acquire physical device present modes "
                             "with code {}!\n",
                             string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_DEVICEVK_CPP>(
                "physical device present modes acquire success!" );
        }
    }

    // This present mode will be used across application, in
    // swapchain in particular. Make choose liitle wiser in future
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
