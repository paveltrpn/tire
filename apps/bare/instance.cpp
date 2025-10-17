
#include <format>
#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vk_enum_string_helper.h>

#include "context_bare.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_INSTANCE_CPP{ true };

#include "config/config.h"

namespace tire::vk {

#define STRING_RESET "\033[0m"
#define STRING_INFO "\033[37m"
#define STRING_WARNING "\033[33m"
#define STRING_ERROR "\033[36m"

namespace {

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
               VkDebugUtilsMessageTypeFlagsEXT messageTypes,
               const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
               void* pUserData ) {
    const std::string message{ pCallbackData->pMessage };

    if ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT ) {
        std::cout << std::format( "{}\n\n",
                                  STRING_INFO + message + STRING_RESET );
    }

    if ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT ) {
        std::cout << std::format( "{}\n\n",
                                  STRING_WARNING + message + STRING_RESET );
    }

    if ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT ) {
        std::cout << std::format( "{}\n\n",
                                  STRING_ERROR + message + STRING_RESET );
    }

    return VK_FALSE;
}

static VkResult vkCreateDebugUtilsMessenger(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger ) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkCreateDebugUtilsMessengerEXT" );
    if ( func != nullptr ) {
        return func( instance, pCreateInfo, pAllocator, pDebugMessenger );
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

}  // namespace

void ContextBare::makeInstance( const std::string surfaceExtension ) {
    const auto configPtr = Config::instance();

    const auto applicationName = configPtr->getString( "application_name" );
    const auto engineName = configPtr->getString( "engine_name" );

    const VkApplicationInfo appInfo{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = applicationName.data(),
        .applicationVersion = VK_MAKE_VERSION( 1, 0, 0 ),
        .pEngineName = engineName.data(),
        .engineVersion = VK_MAKE_VERSION( 1, 0, 0 ),
        .apiVersion = VK_API_VERSION_1_3 };

    // Enumerate instance layers
    uint32_t layersCount;
    if ( const auto err =
             vkEnumerateInstanceLayerProperties( &layersCount, nullptr );
         err != VK_SUCCESS ) {
        log::fatal( "can't enumerate instance layer properties with code: {}\n",
                    string_VkResult( err ) );
    } else {
        log::debug<DEBUG_OUTPUT_INSTANCE_CPP>(
            "vk::Instance === layer properties value: {}", layersCount );
    }

    layerProperties_.resize( layersCount );

    // Collect instance layers
    if ( const auto err = vkEnumerateInstanceLayerProperties(
             &layersCount, layerProperties_.data() );
         err != VK_SUCCESS ) {
        log::fatal( "can't acquire instance layer properties with code: {}\n",
                    string_VkResult( err ) );
    } else {
        log::info( "vk::Instance === layer properties acquired" );
    }

    // Vulkan vlidation layers list to enable
    if ( configPtr->get<bool>( "enable_validation_layers" ) ) {
        desiredValidationLayerList_.push_back( "VK_LAYER_KHRONOS_validation" );

        if ( configPtr->get<bool>( "enable_additional_validation_layers" ) ) {
            desiredValidationLayerList_.emplace_back(
                "VK_LAYER_KHRONOS_profiles" );
            desiredValidationLayerList_.emplace_back(
                "VK_LAYER_KHRONOS_shader_object" );
            desiredValidationLayerList_.emplace_back(
                "VK_LAYER_KHRONOS_synchronization2" );
            desiredValidationLayerList_.emplace_back(
                "VK_LAYER_LUNARG_crash_diagnostic" );
            desiredValidationLayerList_.emplace_back(
                "VK_LAYER_LUNARG_gfxreconstruct" );
            desiredValidationLayerList_.emplace_back(
                "VK_LAYER_LUNARG_monitor" );
            desiredValidationLayerList_.emplace_back(
                "VK_LAYER_LUNARG_screenshot" );

            // Vendor specific layers
            // desiredValidationLayerList_.emplace_back(
            // "VK_LAYER_MESA_device_select" );
            // desiredValidationLayerList_.emplace_back( "VK_LAYER_NV_optimus" );
        }

        // NOTE: "VK_LAYER_RENDERDOC_Capture" must be available in system to use renderdoc

        if ( configPtr->get<bool>( "enable_api_dump_validation_layers" ) ) {
            desiredValidationLayerList_.emplace_back(
                "VK_LAYER_LUNARG_api_dump" );
        }
    }

    const std::vector<VkValidationFeatureEnableEXT>
        validationFeatureEnableList = {
            VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT,
            VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT,
            VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT,
            /*VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT,*/
            VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT };

    VkValidationFeaturesEXT validationFeatures = {
        .sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT,
        .pNext = nullptr,
        .enabledValidationFeatureCount =
            (uint32_t)validationFeatureEnableList.size(),
        .pEnabledValidationFeatures = validationFeatureEnableList.data(),
        .disabledValidationFeatureCount = 0,
        .pDisabledValidationFeatures = nullptr };

    auto debugUtilsMessageTypeFlagBits =
        (VkDebugUtilsMessageTypeFlagBitsEXT)( VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                              VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT );

    auto debugUtilsMessageSeverityFlagBits =
        (VkDebugUtilsMessageSeverityFlagBitsEXT)( VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT );

    if ( configPtr->get<bool>( "verbose_vulkan_output" ) ) {
        debugUtilsMessageTypeFlagBits =
            (VkDebugUtilsMessageTypeFlagBitsEXT)( VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                                  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                                  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT );

        debugUtilsMessageSeverityFlagBits =
            (VkDebugUtilsMessageSeverityFlagBitsEXT)( VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                                      VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                                                      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                                      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT );
    }

    // Debug utils messanger creation info
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

    // Vulkan instance extensions list
    std::vector<const char*> desiredInstanceExtensionsList{};
    desiredInstanceExtensionsList.emplace_back( "VK_KHR_surface" );
    desiredInstanceExtensionsList.emplace_back( surfaceExtension.c_str() );
    // desiredInstanceExtensionsList.emplace_back( "VK_KHR_xlib_surface" );
    // desiredInstanceExtensionsList.emplace_back( "VK_KHR_wayland_surface" );
    if ( configPtr->get<bool>( "enable_validation_layers" ) ) {
        desiredInstanceExtensionsList.emplace_back(
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
    }

    // Instance creation info
    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;

    if ( configPtr->get<bool>( "enable_validation_layers" ) ) {
        instanceCreateInfo.enabledLayerCount =
            static_cast<uint32_t>( desiredValidationLayerList_.size() );
        instanceCreateInfo.ppEnabledLayerNames =
            desiredValidationLayerList_.data();
        instanceCreateInfo.pNext =
            (VkDebugUtilsMessengerCreateInfoEXT*)&debugUtilsMessengerCreateInfo;
    }

    instanceCreateInfo.enabledExtensionCount =
        static_cast<uint32_t>( desiredInstanceExtensionsList.size() );
    instanceCreateInfo.ppEnabledExtensionNames =
        desiredInstanceExtensionsList.data();

    // Enumerate instance extensions properties
    uint32_t extCount{};
    if ( const auto err = vkEnumerateInstanceExtensionProperties(
             nullptr, &extCount, nullptr );
         err != VK_SUCCESS ) {
        log::fatal(
            "can't enumerate instance extension "
            "properties with code: {}\n",
            string_VkResult( err ) );
    } else {
        log::debug<DEBUG_OUTPUT_INSTANCE_CPP>(
            "vk::Instance === extension properties value: {}", extCount );
    }

    extensionProperties_.resize( extCount );

    // Collect instance extensions properties
    if ( const auto err = vkEnumerateInstanceExtensionProperties(
             nullptr, &extCount, extensionProperties_.data() );
         err != VK_SUCCESS ) {
        log::fatal(
            "can't acquire instance extension properties "
            "with code: {}\n",
            string_VkResult( err ) );
    } else {
        log::info( "vk::Instance === extension properties aquired" );
    }

    // Create vulkan instance
    if ( const auto err =
             vkCreateInstance( &instanceCreateInfo, nullptr, &instance_ );
         err != VK_SUCCESS ) {
        log::fatal( "can't create vk instance with code: {}\n",
                    string_VkResult( err ) );
    } else {
        log::info( "vk::Instance === vulkan instance created!" );
    }

    // Create debug utils messanger
    if ( configPtr->get<bool>( "enable_validation_layers" ) ) {
        if ( const auto err = vkCreateDebugUtilsMessenger(
                 instance_, &debugUtilsMessengerCreateInfo, nullptr,
                 &debugMessenger_ );
             err != VK_SUCCESS ) {
            log::fatal( "failed to set up debug messenger with code {}!\n",
                        string_VkResult( err ) );
        } else {
            log::debug<DEBUG_OUTPUT_INSTANCE_CPP>(
                "vk::Instance === vkCreateDebugUtilsMessenger success!" );
        }
    }
}
}  // namespace tire::vk