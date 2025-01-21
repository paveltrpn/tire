
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

    uint32_t layersCount;
    if ( const auto err =
             vkEnumerateInstanceLayerProperties( &layersCount, nullptr );
         err != VK_SUCCESS ) {
        throw std::runtime_error( std::format(
            "can't enumerate instance layer properties with code: {}\n",
            string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_INSTANCEVK_CPP>(
            "vk::Instance === layer properties value: {}", layersCount );
    }

    layerProperties_.resize( layersCount );

    if ( const auto err = vkEnumerateInstanceLayerProperties(
             &layersCount, layerProperties_.data() );
         err != VK_SUCCESS ) {
        throw std::runtime_error( std::format(
            "can't acquire instance layer properties with code: {}\n",
            string_VkResult( err ) ) );
    } else {
        log::info( "vk::Instance === layer properties acquired" );
    }

    // Vulkan vlidation layers list to enable
    // vllist.push_back( "VK_LAYER_INTEL_nullhw" ); // cause crash on device creation

    // Show little fancy window with some useful info.
    // desiredValidationLayerList_.emplace_back( "VK_LAYER_MESA_overlay" );
    desiredValidationLayerList_.emplace_back( "VK_LAYER_NV_optimus" );
    desiredValidationLayerList_.emplace_back(
        "VK_LAYER_VALVE_steam_fossilize_64" );
    desiredValidationLayerList_.emplace_back(
        "VK_LAYER_VALVE_steam_overlay_64" );
    // desiredInstanceValidationLayerList.push_back( "VK_LAYER_KHRONOS_validation" ); // not supported

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
            static_cast<uint32_t>( desiredValidationLayerList_.size() );
        instanceCreateInfo.ppEnabledLayerNames =
            desiredValidationLayerList_.data();
        instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT
                                        *)&debugUtilsMessengerCreateInfo;
    }

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

    // Vulkan instance extensions list
    std::vector<const char *> desiredInstanceExtensionsList{};
    desiredInstanceExtensionsList.emplace_back( "VK_KHR_surface" );
    desiredInstanceExtensionsList.emplace_back( "VK_KHR_xlib_surface" );
    desiredInstanceExtensionsList.emplace_back(
        VK_EXT_DEBUG_REPORT_EXTENSION_NAME );
    desiredInstanceExtensionsList.emplace_back(
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME );

    instanceCreateInfo.enabledExtensionCount =
        static_cast<uint32_t>( desiredInstanceExtensionsList.size() );
    instanceCreateInfo.ppEnabledExtensionNames =
        desiredInstanceExtensionsList.data();

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

VkInstance Instance::handle() const {
    return instance_;
}

std::pair<uint32_t, char const *const *> Instance::validationLayers() const {
    return std::make_pair( desiredValidationLayerList_.size(),
                           desiredValidationLayerList_.data() );
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
