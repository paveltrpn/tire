
#include <vulkan/vk_enum_string_helper.h>

#include "instance.h"
#include "config/config.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_INSTANCEVK_CPP{ true };

namespace tire::vk {

#define STRING_RESET "\033[0m"
#define STRING_INFO "\033[37m"
#define STRING_WARNING "\033[33m"
#define STRING_ERROR "\033[36m"

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
               VkDebugUtilsMessageTypeFlagsEXT messageTypes,
               const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
               void *pUserData ) {
    const std::string message{ pCallbackData->pMessage };

    if ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT ) {
        std::println( "{}", STRING_INFO + message + STRING_RESET );
    }

    if ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT ) {
        std::println( "{}", STRING_WARNING + message + STRING_RESET );
    }

    if ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT ) {
        std::println( "{}", STRING_ERROR + message + STRING_RESET );
    }

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

    const VkApplicationInfo appInfo{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = applicationName.data(),
        .applicationVersion = VK_MAKE_VERSION( 1, 0, 0 ),
        .pEngineName = engineName.data(),
        .engineVersion = VK_MAKE_VERSION( 1, 0, 0 ),
        .apiVersion = VK_API_VERSION_1_0 };

    const auto debugUtilsMessageSeverityFlagBits =
        ( VkDebugUtilsMessageSeverityFlagBitsEXT )(
            // VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT );

    std::vector<std::string> vllist{};
    // vllist.push_back( "VK_LAYER_INTEL_nullhw" );
    vllist.emplace_back( "VK_LAYER_MESA_overlay" );
    vllist.emplace_back( "VK_LAYER_NV_optimus" );
    vllist.emplace_back( "VK_LAYER_VALVE_steam_fossilize_64" );
    vllist.emplace_back( "VK_LAYER_VALVE_steam_overlay_64" );
    // vllist.push_back( "VK_LAYER_KHRONOS_validation" );

    validationLayersNames_ = makeValidationLayersList( vllist );

    const std::vector<VkValidationFeatureEnableEXT>
        validationFeatureEnableList = {
            // VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT,
            VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT,
            VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT };

    VkValidationFeaturesEXT validationFeatures = {
        .sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT,
        .pNext = nullptr,
        .enabledValidationFeatureCount =
            (uint32_t)validationFeatureEnableList.size(),
        .pEnabledValidationFeatures = validationFeatureEnableList.data(),
        .disabledValidationFeatureCount = 0,
        .pDisabledValidationFeatures = nullptr };

    const auto debugUtilsMessageTypeFlagBits =
        ( VkDebugUtilsMessageTypeFlagBitsEXT )(
            // VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT );

    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = &validationFeatures,
        .flags = 0,
        .messageSeverity = (VkDebugUtilsMessageSeverityFlagsEXT)
            debugUtilsMessageSeverityFlagBits,
        .messageType =
            (VkDebugUtilsMessageTypeFlagsEXT)debugUtilsMessageTypeFlagBits,
        .pfnUserCallback = &debugCallback,
        .pUserData = nullptr };

    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;

    if ( configPtr->get<bool>( "enable_validation_layers" ) ) {
        instanceCreateInfo.enabledLayerCount =
            static_cast<uint32_t>( validationLayersNames_.size() );
        instanceCreateInfo.ppEnabledLayerNames = validationLayersNames_.data();
        instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT
                                        *)&debugUtilsMessengerCreateInfo;
    }

    // extensions
    std::vector<std::string> eplist{};
    eplist.emplace_back( "VK_KHR_surface" );
    eplist.emplace_back( "VK_KHR_xlib_surface" );
    eplist.emplace_back( "VK_EXT_debug_report" );
    eplist.emplace_back( "VK_EXT_debug_utils" );
    eplist.emplace_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
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
                 instance_, &debugUtilsMessengerCreateInfo, nullptr,
                 &debugMessenger_ );
             err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to set up debug messenger with code {}!\n",
                             string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_INSTANCEVK_CPP>(
                "vk::Instance === vkCreateDebugUtilsMessenger success!" );
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
