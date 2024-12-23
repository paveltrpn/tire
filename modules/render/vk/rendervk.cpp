
#include <print>
#include <format>

#include <vulkan/vulkan.h>

#include "rendervk.h"
#include "geometry/node.h"

namespace tire {

RenderVK::RenderVK()
    : Render{} {
    const auto configPtr = Config::instance();

    applicationName_ = configPtr->getString( "application_name" );
    engineName_ = configPtr->getString( "engine_name" );
    enableValidationLayers_ =
        configPtr->get<bool>( "enable_validation_layers" );

    try {
        createInstance();
        initPhysicalDevices();
        pickAndCreateDevice( 0 );
    } catch ( const std::runtime_error &e ) {
        throw std::runtime_error( e.what() );
    }
};

RenderVK::~RenderVK() {
    vkDestroyDevice( device_, nullptr );
    vkDestroyInstance( instance_, nullptr );
};

void RenderVK::setSwapInterval( int interval ){

};

void RenderVK::enumerateExtensionProperties() {
    uint32_t extCount;

    vkEnumerateInstanceExtensionProperties( nullptr, &extCount, nullptr );
    extensionProperties_.resize( extCount );
    vkEnumerateInstanceExtensionProperties( nullptr, &extCount,
                                            extensionProperties_.data() );
};

// pass std::nullopt to enable all available exensions
std::vector<char *> RenderVK::makeExtensionsList(
    std::optional<std::vector<std::string>> list ) {
    std::vector<char *> rt{};

    enumerateExtensionProperties();

    if ( list ) {
        for ( const auto &name : list.value() ) {
            auto res = std::find_if( extensionProperties_.begin(),
                                     extensionProperties_.end(),
                                     [name]( auto &ep ) -> bool {
                                         return ep.extensionName == name;
                                     } );
            if ( res != extensionProperties_.end() ) {
                rt.push_back( ( *res ).extensionName );
            } else {
                std::print( "extension \"{}\" not supported\n", name );
            }
        }
    } else {
        for ( auto &ep : extensionProperties_ ) {
            rt.push_back( ep.extensionName );
        }
    }

    return rt;
}

void RenderVK::enumerateValidationLayers() {
    uint32_t layerCount;

    vkEnumerateInstanceLayerProperties( &layerCount, nullptr );
    layerProperties_.resize( layerCount );
    vkEnumerateInstanceLayerProperties( &layerCount, layerProperties_.data() );
}

// pass std::nullopt to enable all avaible validation layers.
// may cause instance creation error, for example:
// "Requested layer "VK_LAYER_VALVE_steam_overlay_32" was wrong bit-type!"
std::vector<char *> RenderVK::makeValidationLayersList(
    std::optional<std::vector<std::string>> list ) {
    std::vector<char *> rt{};

    enumerateValidationLayers();

    if ( list ) {
        for ( const auto &name : list.value() ) {
            auto res = std::find_if(
                layerProperties_.begin(), layerProperties_.end(),
                [name]( auto &lp ) -> bool { return lp.layerName == name; } );
            if ( res != layerProperties_.end() ) {
                rt.push_back( ( *res ).layerName );
            } else {
                std::print( "validation layer \"{}\" not supported\n", name );
            }
        }
    } else {
        for ( auto &lp : layerProperties_ ) {
            rt.push_back( lp.layerName );
        }
    }

    return rt;
}

void RenderVK::createInstance() {
    // VkApplicationInfo
    appInfo_.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo_.pApplicationName = applicationName_.data();
    appInfo_.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
    appInfo_.pEngineName = engineName_.data();
    appInfo_.engineVersion = VK_MAKE_VERSION( 1, 0, 0 );
    appInfo_.apiVersion = VK_API_VERSION_1_0;

    // VkDebugCreateInfo
    dbgCreateInfo_.sType =
        VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    dbgCreateInfo_.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    dbgCreateInfo_.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    dbgCreateInfo_.pfnUserCallback = debugCallback;
    dbgCreateInfo_.pUserData = nullptr;  // Optional

    // VkInstanceCreateInfo
    instanceCreateInfo_.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo_.pApplicationInfo = &appInfo_;

    // validation layers
    std::vector<std::string> vllist{
        "VK_LAYER_INTEL_nullhw", "VK_LAYER_MESA_device_select",
        "VK_LAYER_MESA_overlay", "VK_LAYER_NV_optimus",
        //"VK_LAYER_VALVE_steam_fossilize_32",
        "VK_LAYER_VALVE_steam_fossilize_64",
        //"VK_LAYER_VALVE_steam_overlay_32",
        "VK_LAYER_VALVE_steam_overlay_64" };

    validationLayersNames_ = makeValidationLayersList( vllist );

    if ( enableValidationLayers_ ) {
        instanceCreateInfo_.enabledLayerCount =
            static_cast<uint32_t>( validationLayersNames_.size() );
        instanceCreateInfo_.ppEnabledLayerNames = validationLayersNames_.data();
        instanceCreateInfo_.pNext =
            (VkDebugUtilsMessengerCreateInfoEXT *)&dbgCreateInfo_;
    }

    // extensions
    std::vector<std::string> eplist{ "VK_KHR_surface", "VK_KHR_xlib_surface",
                                     "VK_EXT_debug_report",
                                     "VK_EXT_debug_utils" };

    extensionsNames_ = makeExtensionsList( std::nullopt );

    instanceCreateInfo_.enabledExtensionCount =
        static_cast<uint32_t>( extensionsNames_.size() );
    instanceCreateInfo_.ppEnabledExtensionNames = extensionsNames_.data();

    // instance creation
    auto res = vkCreateInstance( &instanceCreateInfo_, nullptr, &instance_ );

    if ( res != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "can't create vk instance with code: {}\n",
                         static_cast<int>( res ) ) );
    }

    if ( enableValidationLayers_ ) {
        if ( vkCreateDebugUtilsMessenger( instance_, &dbgCreateInfo_, nullptr,
                                          &debugMessenger_ ) != VK_SUCCESS ) {
            throw std::runtime_error( "failed to set up debug messenger!\n" );
        }
    }
}

void RenderVK::initPhysicalDevices() {
    uint32_t devCount{};
    vkEnumeratePhysicalDevices( instance_, &devCount, nullptr );
    if ( devCount == 0 ) {
        throw std::runtime_error( "no vk physical devices in system\n" );
    }

    std::vector<VkPhysicalDevice> physicalDevices( devCount );
    vkEnumeratePhysicalDevices( instance_, &devCount, physicalDevices.data() );

    for ( const auto device : physicalDevices ) {
        // Collect physical devices and its properties
        VkPhysicalDeviceProperties devProps;
        VkPhysicalDeviceFeatures devFeatures;
        vkGetPhysicalDeviceProperties( device, &devProps );
        vkGetPhysicalDeviceFeatures( device, &devFeatures );

        // Collect physical device queue families properies
        uint32_t queueFamilyCount{};
        vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount,
                                                  nullptr );
        std::vector<VkQueueFamilyProperties> qfp( queueFamilyCount );
        vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount,
                                                  qfp.data() );

        physicalDevices_.push_back(
            PhysicalDevice{ device, devProps, devFeatures, qfp } );
    }
}

void RenderVK::pickAndCreateDevice( size_t id ) {
    // Check is physical device suitable, can be done acoording to
    // physical devices properties and physical device queue families properies

    const auto &deviceProps = physicalDevices_[id].devicesProperties;
    if ( !( deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ) ) {
        throw std::runtime_error(
            "picked physical device is not discrete GPU!" );
    }

    // Create a new device instance.
    // A logical device is created as a connection to a physical device.

    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    // queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;

    deviceCreateInfo_.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo_.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo_.queueCreateInfoCount = 1;
    deviceCreateInfo_.pEnabledFeatures = &physicalDevices_[id].devicesFeatures;

    // deviceCreateInfo_.enabledExtensionCount
    //   = static_cast<uint32_t>(extensionsNames_.size());
    // deviceCreateInfo_.ppEnabledExtensionNames = extensionsNames_.data();

    if ( enableValidationLayers_ ) {
        deviceCreateInfo_.enabledLayerCount =
            static_cast<uint32_t>( validationLayersNames_.size() );
        deviceCreateInfo_.ppEnabledLayerNames = validationLayersNames_.data();
    } else {
        deviceCreateInfo_.enabledLayerCount = 0;
    }

    // Create a logical device
    if ( vkCreateDevice( physicalDevices_[id].device, &deviceCreateInfo_,
                         nullptr, &device_ ) != VK_SUCCESS ) {
        throw std::runtime_error( "failed to create logical device!" );
    }

    vkGetDeviceQueue( device_, id, 0, &graphicsQueue_ );
}

}  // namespace tire
