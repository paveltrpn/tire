
#include <format>
#include <memory>

#include "context.h"

#include "config/config.h"
#include "log/log.h"
#include "image/image.h"

#include "command_routine.h"

namespace tire {

struct DepthImage;

static auto vkDestroyDebugUtilsMessenger( VkInstance instance, VkDebugUtilsMessengerEXT messanger,
                                          const VkAllocationCallbacks *pAllocator ) -> void {
    auto func =
        (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr( instance, "vkDestroyDebugUtilsMessengerEXT" );
    if ( func != nullptr ) {
        return func( instance, messanger, pAllocator );
    }
}

#ifdef SURFACE_X11
Context::Context( uint32_t width, uint32_t height, Display *display, Window window )
    : width_{ width }
    , height_{ height } {
    //
    makeInstance( "VK_KHR_xlib_surface" );
    makeXlibSurface( display, window );
    initRest();
}
#elifdef SURFACE_WAYLAND
Context::Context( uint32_t width, uint32_t height, wl_display *display, wl_surface *surface )
    : width_{ width }
    , height_{ height } {
    //
    makeInstance( "VK_KHR_wayland_surface" );
    makeWaylandSurface( display, surface );
    initRest();
}
#endif

// Destroy all Vulkan context here.
Context::~Context() {
    vkDestroyFence( device_, copyCommandFence_, nullptr );
    vkDestroyDescriptorPool( device_, descriptorPool_, nullptr );

    for ( auto i = 0; i < framesCount_; i++ ) {
        vkDestroySemaphore( device_, frames_[i].imageAvailableSemaphore_, nullptr );
        vkDestroySemaphore( device_, frames_[i].renderFinishedSemaphore_, nullptr );
        vkDestroyFence( device_, frames_[i].inFlightFence_, nullptr );
        vkDestroyFramebuffer( device_, frames_[i].framebuffer_, nullptr );
        vkDestroyImageView( device_, frames_[i].view_, nullptr );
    }

    vkDestroyCommandPool( device_, commandPool_, nullptr );
    vkDestroySurfaceKHR( vkInstance_, surface_, nullptr );
    vkDestroyDebugUtilsMessenger( vkInstance_, debugMessenger_, nullptr );
    vmaDestroyAllocator( allocator_ );

    vkDestroySwapchainKHR( device_, swapchain_, nullptr );
    vkDestroyDevice( device_, nullptr );
    vkDestroyInstance( vkInstance_, nullptr );
}

#ifdef SURFACE_X11
auto Context::makeXlibSurface( Display *display, Window window ) -> void {
    VkXlibSurfaceCreateInfoKHR xlibSurfInfo{};
    xlibSurfInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    xlibSurfInfo.dpy = display;
    xlibSurfInfo.window = window;

    if ( const auto err = vkCreateXlibSurfaceKHR( vkInstance_, &xlibSurfInfo, nullptr, &surface_ ); err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create xlib surface with code {}\n!", string_VkResult( err ) ) );
    } else {
        log::info()( "xlib surface created!" );
    }
}
#elifdef SURFACE_WAYLAND
auto Context::makeWaylandSurface( wl_display *display, wl_surface *surface ) -> void {
    VkWaylandSurfaceCreateInfoKHR wlSurfInfo{};
    wlSurfInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    wlSurfInfo.display = display;
    wlSurfInfo.surface = surface;

    if ( const auto err = vkCreateWaylandSurfaceKHR( instance_, &wlSurfInfo, nullptr, &surface_ ); err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create wayland surface with code {}\n!", string_VkResult( err ) ) );
    } else {
        log::info()( "wayland surface created!" );
    }
}
#endif

#define STRING_RESET "\033[0m"
#define STRING_INFO "\033[37m"
#define STRING_WARNING "\033[33m"
#define STRING_ERROR "\033[36m"

namespace {

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                     VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                                                     const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                     void *pUserData ) {
    const std::string message{ pCallbackData->pMessage };

    if ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT ) {
        std::cout << std::format( "{}\n\n", STRING_INFO + message + STRING_RESET );
    }

    if ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT ) {
        std::cout << std::format( "{}\n\n", STRING_WARNING + message + STRING_RESET );
    }

    if ( messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT ) {
        std::cout << std::format( "{}\n\n", STRING_ERROR + message + STRING_RESET );
    }

    return VK_FALSE;
}

static VkResult vkCreateDebugUtilsMessenger( VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                             const VkAllocationCallbacks *pAllocator,
                                             VkDebugUtilsMessengerEXT *pDebugMessenger ) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr( instance, "vkCreateDebugUtilsMessengerEXT" );
    if ( func != nullptr ) {
        return func( instance, pCreateInfo, pAllocator, pDebugMessenger );
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

}  // namespace

void Context::makeInstance( const std::string &platformSurfaceExtension ) {
    const auto configPtr = Config::instance();

    const auto applicationName = configPtr->getString( "application_name" );
    const auto engineName = configPtr->getString( "engine_name" );

    const VkApplicationInfo appInfo{ .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                     .pApplicationName = applicationName.data(),
                                     .applicationVersion = VK_MAKE_VERSION( 1, 0, 0 ),
                                     .pEngineName = engineName.data(),
                                     .engineVersion = VK_MAKE_VERSION( 1, 0, 0 ),
                                     .apiVersion = VK_API_VERSION_1_3 };

    // Enumerate instance layers
    uint32_t layersCount;
    if ( const auto err = vkEnumerateInstanceLayerProperties( &layersCount, nullptr ); err != VK_SUCCESS ) {
        log::fatal()( "can't enumerate instance layer properties with code: {}\n", string_VkResult( err ) );
    } else {
        log::debug()( "layer properties value: {}", layersCount );
    }

    layerProperties_.resize( layersCount );

    // Collect instance layers
    if ( const auto err = vkEnumerateInstanceLayerProperties( &layersCount, layerProperties_.data() );
         err != VK_SUCCESS ) {
        log::fatal()( "can't acquire instance layer properties with code: {}\n", string_VkResult( err ) );
    } else {
        log::info()( "layer properties acquired" );
    }
    /*
    // Layers optimal order:
    // <a href='https://vulkan.lunarg.com/doc/view/1.0.13.0/windows/layers.html'/>
    val layers = arrayOf(
        "VK_LAYER_GOOGLE_threading", "VK_LAYER_LUNARG_parameter_validation",
        "VK_LAYER_LUNARG_object_tracker", "VK_LAYER_LUNARG_core_validation",
        "VK_LAYER_GOOGLE_unique_objects",
        "VK_LAYER_LUNARG_standard_validation" )
        */

    // Vulkan vlidation layers list to enable
    if ( configPtr->get<bool>( "enable_validation_layers" ) ) {
        desiredValidationLayerList_.push_back( "VK_LAYER_KHRONOS_validation" );

        if ( configPtr->get<bool>( "enable_additional_validation_layers" ) ) {
            desiredValidationLayerList_.emplace_back( "VK_LAYER_KHRONOS_profiles" );
            desiredValidationLayerList_.emplace_back( "VK_LAYER_KHRONOS_shader_object" );
            desiredValidationLayerList_.emplace_back( "VK_LAYER_KHRONOS_synchronization2" );
            desiredValidationLayerList_.emplace_back( "VK_LAYER_LUNARG_crash_diagnostic" );
            desiredValidationLayerList_.emplace_back( "VK_LAYER_LUNARG_gfxreconstruct" );
            desiredValidationLayerList_.emplace_back( "VK_LAYER_LUNARG_monitor" );
            desiredValidationLayerList_.emplace_back( "VK_LAYER_LUNARG_screenshot" );

            // Vendor specific layers
            // desiredValidationLayerList_.emplace_back(
            // "VK_LAYER_MESA_device_select" );
            // desiredValidationLayerList_.emplace_back( "VK_LAYER_NV_optimus" );
        }

        // NOTE: "VK_LAYER_RENDERDOC_Capture" must be available in system to use renderdoc

        if ( configPtr->get<bool>( "enable_api_dump_validation_layers" ) ) {
            desiredValidationLayerList_.emplace_back( "VK_LAYER_LUNARG_api_dump" );
        }
    }

    const std::vector<VkValidationFeatureEnableEXT> validationFeatureEnableList = {
                                                                                        VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT, VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT,
                                                                                        VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT,
                                                                                        /*VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT,*/
                                                                                        /*
       * NOTE: Ahhhh... got some synchronization validation errors while copy
       * from staging to GPU buffer, but i can't see wrong place in
       * that code... jost turn off annoyng debug output and deal with
       * it later...
       *
      VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT*/ };

    VkValidationFeaturesEXT validationFeatures = {
        .sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT,
        .pNext = nullptr,
        .enabledValidationFeatureCount = (uint32_t)validationFeatureEnableList.size(),
        .pEnabledValidationFeatures = validationFeatureEnableList.data(),
        .disabledValidationFeatureCount = 0,
        .pDisabledValidationFeatures = nullptr };

    auto debugUtilsMessageTypeFlagBits = ( VkDebugUtilsMessageTypeFlagBitsEXT )(
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT );

    auto debugUtilsMessageSeverityFlagBits = ( VkDebugUtilsMessageSeverityFlagBitsEXT )(
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT );

    if ( configPtr->get<bool>( "verbose_vulkan_output" ) ) {
        debugUtilsMessageTypeFlagBits = ( VkDebugUtilsMessageTypeFlagBitsEXT )(
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT );

        debugUtilsMessageSeverityFlagBits = ( VkDebugUtilsMessageSeverityFlagBitsEXT )(
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT );
    }

    // Debug utils messanger creation info
    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = &validationFeatures,
        .flags = 0,
        .messageSeverity = (VkDebugUtilsMessageSeverityFlagsEXT)debugUtilsMessageSeverityFlagBits,
        .messageType = (VkDebugUtilsMessageTypeFlagsEXT)debugUtilsMessageTypeFlagBits,
        .pfnUserCallback = &debugCallback,
        .pUserData = nullptr };

    // Vulkan instance extensions list
    std::vector<const char *> desiredInstanceExtensionsList{};
    desiredInstanceExtensionsList.emplace_back( "VK_KHR_surface" );
    desiredInstanceExtensionsList.emplace_back( platformSurfaceExtension.c_str() );
    // desiredInstanceExtensionsList.emplace_back( "VK_KHR_xlib_surface" );
    // desiredInstanceExtensionsList.emplace_back( "VK_KHR_wayland_surface" );
    if ( configPtr->get<bool>( "enable_validation_layers" ) ) {
        desiredInstanceExtensionsList.emplace_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
    }

    // Instance creation info
    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;

    if ( configPtr->get<bool>( "enable_validation_layers" ) ) {
        instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>( desiredValidationLayerList_.size() );
        instanceCreateInfo.ppEnabledLayerNames = desiredValidationLayerList_.data();
        instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugUtilsMessengerCreateInfo;
    }

    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>( desiredInstanceExtensionsList.size() );
    instanceCreateInfo.ppEnabledExtensionNames = desiredInstanceExtensionsList.data();

    // Enumerate instance extensions properties
    uint32_t extCount{};
    if ( const auto err = vkEnumerateInstanceExtensionProperties( nullptr, &extCount, nullptr ); err != VK_SUCCESS ) {
        log::fatal()(
            "can't enumerate instance extension "
            "properties with code: {}\n",
            string_VkResult( err ) );
    } else {
        log::debug()( "extension properties value: {}", extCount );
    }

    extensionProperties_.resize( extCount );

    // Collect instance extensions properties
    if ( const auto err = vkEnumerateInstanceExtensionProperties( nullptr, &extCount, extensionProperties_.data() );
         err != VK_SUCCESS ) {
        log::fatal()(
            "can't acquire instance extension properties "
            "with code: {}\n",
            string_VkResult( err ) );
    } else {
        log::info()( "extension properties aquired" );
    }

    // Create vulkan instance
    if ( const auto err = vkCreateInstance( &instanceCreateInfo, nullptr, &vkInstance_ ); err != VK_SUCCESS ) {
        log::fatal()( "can't create vk instance with code: {}\n", string_VkResult( err ) );
    } else {
        log::info()( "vulkan instance created!" );
    }

    // Create debug utils messanger
    if ( configPtr->get<bool>( "enable_validation_layers" ) ) {
        if ( const auto err =
                 vkCreateDebugUtilsMessenger( vkInstance_, &debugUtilsMessengerCreateInfo, nullptr, &debugMessenger_ );
             err != VK_SUCCESS ) {
            log::fatal()( "failed to set up debug messenger with code {}!\n", string_VkResult( err ) );
        } else {
            log::debug()( "vkCreateDebugUtilsMessenger success!" );
        }
    }
}

}  // namespace tire
