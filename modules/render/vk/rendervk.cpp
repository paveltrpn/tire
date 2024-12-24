
#include <print>
#include <format>

#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#include <set>

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
        createSurface();
        initPhysicalDevices();
        pickAndCreateDevice( 0 );
    } catch ( const std::runtime_error &e ) {
        throw std::runtime_error( e.what() );
    }
};

RenderVK::~RenderVK() {
    vkDestroyDevice( device_, nullptr );
    vkDestroySurfaceKHR( instance_, surface_, nullptr );
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
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = applicationName_.data();
    appInfo.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
    appInfo.pEngineName = engineName_.data();
    appInfo.engineVersion = VK_MAKE_VERSION( 1, 0, 0 );
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkDebugUtilsMessengerCreateInfoEXT dbgCreateInfo{};
    dbgCreateInfo.sType =
        VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    dbgCreateInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    dbgCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    dbgCreateInfo.pfnUserCallback = debugCallback;
    dbgCreateInfo.pUserData = nullptr;  // Optional

    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;

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
        instanceCreateInfo.enabledLayerCount =
            static_cast<uint32_t>( validationLayersNames_.size() );
        instanceCreateInfo.ppEnabledLayerNames = validationLayersNames_.data();
        instanceCreateInfo.pNext =
            (VkDebugUtilsMessengerCreateInfoEXT *)&dbgCreateInfo;
    }

    // extensions
    std::vector<std::string> eplist{ "VK_KHR_surface", "VK_KHR_xlib_surface",
                                     "VK_EXT_debug_report",
                                     "VK_EXT_debug_utils" };

    extensionsNames_ = makeExtensionsList( std::nullopt );

    instanceCreateInfo.enabledExtensionCount =
        static_cast<uint32_t>( extensionsNames_.size() );
    instanceCreateInfo.ppEnabledExtensionNames = extensionsNames_.data();

    // instance creation
    const auto err =
        vkCreateInstance( &instanceCreateInfo, nullptr, &instance_ );

    if ( err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "can't create vk instance with code: {}\n",
                         string_VkResult( err ) ) );
    } else {
        log::info( "vulkan instance created!" );
    }

    if ( enableValidationLayers_ ) {
        const auto err = vkCreateDebugUtilsMessenger(
            instance_, &dbgCreateInfo, nullptr, &debugMessenger_ );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to set up debug messenger with code {}!\n",
                             string_VkResult( err ) ) );
        } else {
            log::info( "vkCreateDebugUtilsMessenger success!" );
        }
    }
}

void RenderVK::initPhysicalDevices() {
    uint32_t devCount{};
    vkEnumeratePhysicalDevices( instance_, &devCount, nullptr );
    if ( devCount == 0 ) {
        throw std::runtime_error( "no vk physical devices in system\n" );
    }

    log::info( "vulkan devices count: {}", devCount );

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

        physicalDevices_.emplace_back(
            PhysicalDevice{ .device = device,
                            .properties = devProps,
                            .features = devFeatures,
                            .queueFamilyProperties = qfp } );

        log::info( "    devices name: {}", devProps.deviceName );
    }
}

void RenderVK::pickAndCreateDevice( size_t id ) {
    // Check is physical device suitable, can be done acoording to
    // physical devices properties and physical device queue families properies

    log::info( "pick device: {}", physicalDevices_[id].properties.deviceName );

    const auto &deviceProps = physicalDevices_[id].properties;
    if ( !( deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ) ) {
        throw std::runtime_error(
            "picked physical device is not discrete GPU!" );
    }

    // Create a new device instance.
    // A logical device is created as a connection to a physical device.

    uint32_t graphicsFamily{};
    uint32_t presentFamily{};
    int i{};
    for ( const auto &queueFamily :
          physicalDevices_[id].queueFamilyProperties ) {
        if ( queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT ) {
            graphicsFamily = i;
        }
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR( physicalDevices_[id].device, i,
                                              surface_, &presentSupport );
        if ( presentSupport ) {
            presentFamily = i;
        }
        i++;
    }

    log::info( "graphics family: {}", graphicsFamily );
    log::info( "present family: {}", presentFamily );

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    const std::set<uint32_t> uniqueQueueFamilies = { graphicsFamily,
                                                     presentFamily };

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

    // TODO: crash
    // deviceCreateInfo_.enabledExtensionCount =
    // static_cast<uint32_t>( extensionsNames_.size() );
    // deviceCreateInfo_.ppEnabledExtensionNames = extensionsNames_.data();

    if ( enableValidationLayers_ ) {
        deviceCreateInfo.enabledLayerCount =
            static_cast<uint32_t>( validationLayersNames_.size() );
        deviceCreateInfo.ppEnabledLayerNames = validationLayersNames_.data();
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

    vkGetDeviceQueue( device_, graphicsFamily, 0, &graphicsQueue_ );
    vkGetDeviceQueue( device_, presentFamily, 0, &presentQueue_ );

    {
        const auto err = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            physicalDevices_[id].device, surface_, &surfaceCapabilities_ );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error( std::format(
                "failed to obtain surface capabilities with code {}!\n",
                string_VkResult( err ) ) );
        } else {
            log::info( "vkGetPhysicalDeviceSurfaceCapabilitiesKHR success!" );
        }
    }
    // displaySurfaceCapabilities();
}

void RenderVK::createSurface() {
    VkXlibSurfaceCreateInfoKHR xlibSurfInfo{};
    xlibSurfInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    xlibSurfInfo.dpy = display_;
    xlibSurfInfo.window = window_;
    const auto res =
        vkCreateXlibSurfaceKHR( instance_, &xlibSurfInfo, nullptr, &surface_ );
    if ( res != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create xlib surface with code {}\n!",
                         string_VkResult( res ) ) );
    } else {
        log::info( "surface create success!" );
    }
}

}  // namespace tire
