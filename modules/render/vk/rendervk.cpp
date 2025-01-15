
#include <print>
#include <format>

#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#include <set>

#include "rendervk.h"

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_RENDERVK_CPP{ true };

namespace tire {
RenderVK::RenderVK()
    : Render{} {
    try {
        instance_ = std::make_unique<vk::Instance>();
        surface_ = std::make_unique<vk::Surface>( display_, window_,
                                                  instance_->instance() );

        initPhysicalDevices();

        pickAndCreateDevice( 0 );

        // valid only after logical device creation
        const auto basePath = Config::instance()->getBasePath().string();
        shaderStorage_ = std::make_unique<vk::ShaderStorage>( device_ );
        shaderStorage_->add( basePath + "/assets/shaders/001_shader_vert.spv",
                             "001_shader_vert" );
        shaderStorage_->add( basePath + "/assets/shaders/001_shader_frag.spv",
                             "001_shader_frag" );

        createSwapchain();
        createImageViews();

        pipelineSimple_ = std::make_unique<vk::PiplineSimple>( device_ );
        pipelineSimple_->initFixed();
        pipelineSimple_->initProgable(
            shaderStorage_->get( "001_shader_vert" ),
            shaderStorage_->get( "001_shader_frag" ) );
        pipelineSimple_->initLayout();
        pipelineSimple_->initRenderPass( swapChainImageFormat_ );
        pipelineSimple_->initPipeline();
        pipelineSimple_->initFixed();

        createFramebuffers();

        commandPool_ = std::make_unique<CommandPool>( device_ );
        commandPool_->init( graphicsFamily_ );

        cBufs_.reserve( 2 );
        for ( auto i = 0; i < cBufs_.capacity(); ++i ) {
            cBufs_.push_back( std::make_unique<CommandBuffer>(
                device_, commandPool_.get() ) );
        }

        createSyncObjects();

    } catch ( const std::runtime_error &e ) {
        throw std::runtime_error( e.what() );
    }
};

RenderVK::~RenderVK() {
    // preserve destruction order, only before logical device
    shaderStorage_.reset( nullptr );
    pipelineSimple_.reset( nullptr );
    commandPool_.reset( nullptr );
    for ( auto i = 0; i < cBufs_.size(); ++i ) {
        cBufs_[i].release();
    }

    for ( auto i = 0; i < 2; i++ ) {
        vkDestroySemaphore( device_, imageAvailableSemaphores_[i], nullptr );
        vkDestroySemaphore( device_, renderFinishedSemaphores_[i], nullptr );
        vkDestroyFence( device_, inFlightFences_[i], nullptr );
    }

    for ( auto framebuffer : framebuffers_ ) {
        vkDestroyFramebuffer( device_, framebuffer, nullptr );
    }
    vkDestroySwapchainKHR( device_, swapChain_, nullptr );
    vkDestroyDevice( device_, nullptr );
};

void RenderVK::scene( const std::filesystem::path &path ) {
    scene_ = std::make_shared<vk::Scene>( path, loop_ );
}

void RenderVK::setSwapInterval( int interval ){

};

void RenderVK::initPhysicalDevices() {
    uint32_t devCount{};

    {
        const auto err = vkEnumeratePhysicalDevices( instance_->instance(),
                                                     &devCount, nullptr );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "can't enumerate physical devices with code: {}\n",
                             string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_RENDERVK_CPP>(
                "physical devices enumerate success, count: {}", devCount );
        }
    }

    if ( devCount == 0 ) {
        throw std::runtime_error( "no vulkan physical devices in system\n" );
    }

    std::vector<VkPhysicalDevice> physicalDevices( devCount );

    {
        const auto err = vkEnumeratePhysicalDevices(
            instance_->instance(), &devCount, physicalDevices.data() );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "can't acquire physical devices with code: {}\n",
                             string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_RENDERVK_CPP>(
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
        log::debug<DEBUG_OUTPUT_RENDERVK_CPP>( "device queue family count: {}",
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
                log::debug<DEBUG_OUTPUT_RENDERVK_CPP>(
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
                log::debug<DEBUG_OUTPUT_RENDERVK_CPP>(
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

void RenderVK::pickAndCreateDevice( size_t id ) {
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
            physicalDevices_[id].device, i, surface_->surface(),
            &presentSupport );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to get device surface support for "
                             "presentation with code {}!\n",
                             string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_RENDERVK_CPP>(
                "device surface support for "
                "presentation acquire success!" );
        }

        if ( presentSupport ) {
            presentFamily_ = i;
        }
        i++;
    }

    log::debug<DEBUG_OUTPUT_RENDERVK_CPP>( "graphics family: {}",
                                           graphicsFamily_ );
    log::debug<DEBUG_OUTPUT_RENDERVK_CPP>( "present family: {}",
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
            physicalDevices_[id].device, surface_->surface(),
            &surfaceCapabilities_ );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error( std::format(
                "failed to obtain surface capabilities with code {}!\n",
                string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_RENDERVK_CPP>(
                "physical device surface capabilities acquire success!" );
        }
    }

    // physical device surface formats
    uint32_t formatCount;
    {
        const auto err = vkGetPhysicalDeviceSurfaceFormatsKHR(
            physicalDevices_[id].device, surface_->surface(), &formatCount,
            nullptr );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to obtain physical device surface formats "
                             "count with code {}!\n",
                             string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_RENDERVK_CPP>(
                "physical device surface formats count obtain success, "
                "count: "
                "{}",
                formatCount );
        }
    }

    surfaceFormats_.resize( formatCount );

    {
        const auto err = vkGetPhysicalDeviceSurfaceFormatsKHR(
            physicalDevices_[id].device, surface_->surface(), &formatCount,
            surfaceFormats_.data() );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error( std::format(
                "failed to acquire physical device surface formats "
                "count with code {}!\n",
                string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_RENDERVK_CPP>(
                "physical device surface formats acquire success!" );
        }
    }

    // physical device present modes
    uint32_t presentModeCount;
    {
        const auto err = vkGetPhysicalDeviceSurfacePresentModesKHR(
            physicalDevices_[id].device, surface_->surface(), &presentModeCount,
            nullptr );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to obtain physical device present modes "
                             "count with code {}!\n",
                             string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_RENDERVK_CPP>(
                "physical device present modes count obtain success, "
                "count: "
                "{}",
                presentModeCount );
        }
    }

    presentModes_.resize( presentModeCount );

    {
        const auto err = vkGetPhysicalDeviceSurfacePresentModesKHR(
            physicalDevices_[id].device, surface_->surface(), &presentModeCount,
            presentModes_.data() );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to acquire physical device present modes "
                             "with code {}!\n",
                             string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_RENDERVK_CPP>(
                "physical device present modes acquire success!" );
        }
    }

    // displaySurfaceCapabilities();
}

void RenderVK::createSwapchain() {
    VkExtent2D extent = surfaceCapabilities_.currentExtent;
    VkSurfaceFormatKHR surfaceFormat =
        surfaceFormats_[0];                           // TODO: choose wisely
    VkPresentModeKHR presentMode = presentModes_[0];  // TODO: choose wisely

    // However, simply sticking to this minimum means that we may sometimes have
    // to wait on the driver to complete internal operations before we can acquire
    // another image to render to. Therefore it is recommended to request at least one
    // more image than the minimum:
    uint32_t imageCount = surfaceCapabilities_.minImageCount + 1;

    // We should also make sure to not exceed the maximum number of images while
    // doing this, where 0 is a special value that means that there is no maximum:
    if ( surfaceCapabilities_.maxImageCount > 0 &&
         imageCount > surfaceCapabilities_.maxImageCount ) {
        imageCount = surfaceCapabilities_.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface_->surface();

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // TODO: pass it from pickAndCreateDevice()
    uint32_t graphicsFamily{ 0 };
    uint32_t presentFamily{ 2 };
    uint32_t queueFamilyIndices[] = { graphicsFamily, presentFamily };
    if ( graphicsFamily != presentFamily ) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;      // Optional
        createInfo.pQueueFamilyIndices = nullptr;  // Optional
    }

    createInfo.preTransform = surfaceCapabilities_.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    {
        const auto err =
            vkCreateSwapchainKHR( device_, &createInfo, nullptr, &swapChain_ );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to create swapchain code {}\n!",
                             string_VkResult( err ) ) );
        } else {
            log::info( "vulkan swapchain created!" );
        }
    }

    // swapchain images itself
    uint32_t swapchainImageCount{};
    {
        const auto err = vkGetSwapchainImagesKHR(
            device_, swapChain_, &swapchainImageCount, nullptr );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error( std::format(
                "failed to get swapchain images count with code {}\n!",
                string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_RENDERVK_CPP>(
                "vulkan swapchain images count: {}", swapchainImageCount );
        }
    }

    swapChainImages_.resize( imageCount );

    {
        const auto err = vkGetSwapchainImagesKHR(
            device_, swapChain_, &imageCount, swapChainImages_.data() );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to get swapchain images with code {}\n!",
                             string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_RENDERVK_CPP>(
                "vulkan swapchain images acquired!" );
        }
    }

    swapChainImageFormat_ = surfaceFormat.format;
    swapChainExtent_ = extent;
}

void RenderVK::createImageViews() {
    swapChainImageViews_.resize( swapChainImages_.size() );
    for ( size_t i = 0; i < swapChainImages_.size(); i++ ) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages_[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat_;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        const auto err = vkCreateImageView( device_, &createInfo, nullptr,
                                            &swapChainImageViews_[i] );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error( std::format(
                "failed to create swapchain image views with code {}\n!",
                string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_RENDERVK_CPP>(
                "swapchain image view {} created!", i );
        }
    }
}

void RenderVK::createFramebuffers() {
    framebuffers_.resize( swapChainImageViews_.size() );
    for ( size_t i = 0; i < swapChainImageViews_.size(); i++ ) {
        VkImageView attachments[] = { swapChainImageViews_[i] };
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = pipelineSimple_->getRenderPass();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChainExtent_.width;
        framebufferInfo.height = swapChainExtent_.height;
        framebufferInfo.layers = 1;

        const auto err = vkCreateFramebuffer( device_, &framebufferInfo,
                                              nullptr, &framebuffers_[i] );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to create framebuffer at {} with code {}!",
                             i, string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_RENDERVK_CPP>( "framebuffer {} created!",
                                                   i );
        }
    }
}

void RenderVK::createSyncObjects() {
    imageAvailableSemaphores_.resize( 2 );
    renderFinishedSemaphores_.resize( 2 );
    inFlightFences_.resize( 2 );

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for ( size_t i = 0; i < 2; i++ ) {
        if ( vkCreateSemaphore( device_, &semaphoreInfo, nullptr,
                                &imageAvailableSemaphores_[i] ) != VK_SUCCESS ||
             vkCreateSemaphore( device_, &semaphoreInfo, nullptr,
                                &renderFinishedSemaphores_[i] ) != VK_SUCCESS ||
             vkCreateFence( device_, &fenceInfo, nullptr,
                            &inFlightFences_[i] ) != VK_SUCCESS ) {
            throw std::runtime_error( "failed to create semaphores!" );
        }
    }
}

}  // namespace tire
