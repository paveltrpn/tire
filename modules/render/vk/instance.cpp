
#include <vulkan/vk_enum_string_helper.h>

#include "instance.h"
#include "config/config.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_INSTANCEVK_CPP{ true };

namespace tire::vk {

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
               VkDebugUtilsMessageTypeFlagsEXT messageType,
               const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
               void *pUserData ) {
    std::print( R"(validation layer debug:		{}
)",
                pCallbackData->pMessage );
    return VK_FALSE;
}

static VkResult vkCreateDebugUtilsMessenger(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger ) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkCreateDebugUtilsMessengerEXT" );
    if ( func != nullptr ) {
        return func( instance, pCreateInfo, pAllocator, pDebugMessenger );
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

Instance::Instance() {
    const auto configPtr = Config::instance();
    const auto applicationName = configPtr->getString( "application_name" );
    const auto engineName = configPtr->getString( "engine_name" );

    appInfo_.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo_.pApplicationName = applicationName.data();
    appInfo_.applicationVersion = VK_MAKE_VERSION( 1, 0, 0 );
    appInfo_.pEngineName = engineName.data();
    appInfo_.engineVersion = VK_MAKE_VERSION( 1, 0, 0 );
    appInfo_.apiVersion = VK_API_VERSION_1_0;

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

    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo_;

    // validation layers
    std::vector<std::string> vllist{ "VK_LAYER_INTEL_nullhw",
                                     "VK_LAYER_MESA_device_select",
                                     "VK_LAYER_MESA_overlay",
                                     "VK_LAYER_NV_optimus",
                                     "VK_LAYER_VALVE_steam_fossilize_64",
                                     "VK_LAYER_VALVE_steam_overlay_64" };
    validationLayersNames_ = makeValidationLayersList( vllist );

    if ( configPtr->get<bool>( "enable_validation_layers" ) ) {
        instanceCreateInfo.enabledLayerCount =
            static_cast<uint32_t>( validationLayersNames_.size() );
        instanceCreateInfo.ppEnabledLayerNames = validationLayersNames_.data();
        instanceCreateInfo.pNext =
            (VkDebugUtilsMessengerCreateInfoEXT *)&dbgCreateInfo_;
    }

    // extensions
    std::vector<std::string> eplist{ "VK_KHR_surface", "VK_KHR_xlib_surface",
                                     "VK_EXT_debug_report",
                                     "VK_EXT_debug_utils" };
    extensionsNames_ = makeExtensionsList( eplist );

    instanceCreateInfo.enabledExtensionCount =
        static_cast<uint32_t>( extensionsNames_.size() );
    instanceCreateInfo.ppEnabledExtensionNames = extensionsNames_.data();

    // instance creation
    if ( const auto err =
             vkCreateInstance( &instanceCreateInfo, nullptr, &instance_ );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "can't create vk instance with code: {}\n",
                         string_VkResult( err ) ) );
    } else {
        log::info( "vk::Instance === vulkan instance created!" );
    }

    if ( configPtr->get<bool>( "enable_validation_layers" ) ) {
        if ( const auto err = vkCreateDebugUtilsMessenger(
                 instance_, &dbgCreateInfo_, nullptr, &debugMessenger_ );
             err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to set up debug messenger with code {}!\n",
                             string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_INSTANCEVK_CPP>(
                "vkCreateDebugUtilsMessenger success!" );
        }
    }
}

Instance::~Instance() {
    vkDestroyInstance( instance_, nullptr );
}

// pass std::nullopt to enable all available exensions
std::vector<char *> Instance::makeExtensionsList(
    std::optional<std::vector<std::string>> list ) {
    std::vector<char *> rt{};
    uint32_t extCount{};

    if ( const auto err = vkEnumerateInstanceExtensionProperties(
             nullptr, &extCount, nullptr );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "can't enumerate instance extension "
                         "properties with code: {}\n",
                         string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_INSTANCEVK_CPP>(
            "vk::Instance === extension properties value: {}", extCount );
    }

    extensionProperties_.resize( extCount );

    if ( const auto err = vkEnumerateInstanceExtensionProperties(
             nullptr, &extCount, extensionProperties_.data() );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "can't acquire instance extension properties "
                         "with code: {}\n",
                         string_VkResult( err ) ) );
    } else {
        log::info( "vk::Instance === extension properties aquired" );
    }

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

// pass std::nullopt to enable all avaible validation layers.
// may cause instance creation error, for example:
// "Requested layer "VK_LAYER_VALVE_steam_overlay_32" was wrong bit-type!"
std::vector<char *> Instance::makeValidationLayersList(
    std::optional<std::vector<std::string>> list ) {
    std::vector<char *> rt{};
    uint32_t layerCount;

    if ( const auto err =
             vkEnumerateInstanceLayerProperties( &layerCount, nullptr );
         err != VK_SUCCESS ) {
        throw std::runtime_error( std::format(
            "can't enumerate instance layer properties with code: {}\n",
            string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_INSTANCEVK_CPP>(
            "vk::Instance === layer properties value: {}", layerCount );
    }

    layerProperties_.resize( layerCount );

    if ( const auto err = vkEnumerateInstanceLayerProperties(
             &layerCount, layerProperties_.data() );
         err != VK_SUCCESS ) {
        throw std::runtime_error( std::format(
            "can't acquire instance layer properties with code: {}\n",
            string_VkResult( err ) ) );
    } else {
        log::info( "vk::Instance === layer properties acquired" );
    }

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

VkInstance Instance::handle() const {
    return instance_;
}

std::pair<uint32_t, char *const *const> Instance::validationLayersInfo() const {
    return std::make_pair( validationLayersNames_.size(),
                           validationLayersNames_.data() );
}

void Instance::info() const {
    std::print(
        R"(Instance extensions count: {}
==========================
)",
        extensionProperties_.size() );
    for ( auto &prop : extensionProperties_ ) {
        std::print( R"(	{} | revision: {}
)",
                    prop.extensionName, prop.specVersion );
    }

    std::print(
        R"(Layers count: {}
=============
)",
        layerProperties_.size() );
    for ( auto &layer : layerProperties_ ) {
        std::print( R"(	{} | specVersion: {}
    {}
)",
                    layer.layerName, layer.specVersion, layer.description );
    }
}

}  // namespace tire::vk
