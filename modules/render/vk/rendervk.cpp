
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
        device_ = std::make_unique<vk::Device>( instance_->instance(),
                                                surface_->surface() );

        device_->pickAndCreateDevice( instance_.get(), 0 );

        // valid only after logical device creation
        const auto basePath = Config::instance()->getBasePath().string();
        shaderStorage_ =
            std::make_unique<vk::ShaderStorage>( device_->device() );
        shaderStorage_->add( basePath + "/assets/shaders/001_shader_vert.spv",
                             "001_shader_vert" );
        shaderStorage_->add( basePath + "/assets/shaders/001_shader_frag.spv",
                             "001_shader_frag" );

        createSwapchain();
        createImageViews();

        pipelineSimple_ =
            std::make_unique<vk::PiplineSimple>( device_->device() );
        pipelineSimple_->initFixed();
        pipelineSimple_->initProgable(
            shaderStorage_->get( "001_shader_vert" ),
            shaderStorage_->get( "001_shader_frag" ) );
        pipelineSimple_->initLayout();
        pipelineSimple_->initRenderPass( swapChainImageFormat_ );
        pipelineSimple_->initPipeline();
        pipelineSimple_->initFixed();

        createFramebuffers();

        commandPool_ = std::make_unique<CommandPool>( device_->device() );
        commandPool_->init( device_->graphicsFamily() );

        cBufs_.reserve( 2 );
        for ( auto i = 0; i < cBufs_.capacity(); ++i ) {
            cBufs_.push_back( std::make_unique<CommandBuffer>(
                device_->device(), commandPool_.get() ) );
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
        vkDestroySemaphore( device_->device(), imageAvailableSemaphores_[i],
                            nullptr );
        vkDestroySemaphore( device_->device(), renderFinishedSemaphores_[i],
                            nullptr );
        vkDestroyFence( device_->device(), inFlightFences_[i], nullptr );
    }

    for ( auto framebuffer : framebuffers_ ) {
        vkDestroyFramebuffer( device_->device(), framebuffer, nullptr );
    }
    vkDestroySwapchainKHR( device_->device(), swapChain_, nullptr );
};

void RenderVK::scene( const std::filesystem::path &path ) {
    scene_ = std::make_shared<vk::Scene>( path, loop_ );
}

void RenderVK::setSwapInterval( int interval ){

};

void RenderVK::createSwapchain() {
    VkExtent2D extent = VkExtent2D{ static_cast<uint32_t>( width_ ),
                                    static_cast<uint32_t>( height_ ) };

    VkSurfaceFormatKHR surfaceFormat =
        device_->surfaceFormat( 0 );  // TODO: choose wisely
    VkPresentModeKHR presentMode =
        device_->presentMode( 0 );  // TODO: choose wisely

    const auto surfaceCapabilities = device_->surfaceCapabilities();
    // However, simply sticking to this minimum means that we may sometimes have
    // to wait on the driver to complete internal operations before we can acquire
    // another image to render to. Therefore it is recommended to request at least one
    // more image than the minimum:
    uint32_t imageCount = surfaceCapabilities.minImageCount + 1;

    // We should also make sure to not exceed the maximum number of images while
    // doing this, where 0 is a special value that means that there is no maximum:
    if ( surfaceCapabilities.maxImageCount > 0 &&
         imageCount > surfaceCapabilities.maxImageCount ) {
        imageCount = surfaceCapabilities.maxImageCount;
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

    createInfo.preTransform = surfaceCapabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    {
        const auto err = vkCreateSwapchainKHR( device_->device(), &createInfo,
                                               nullptr, &swapChain_ );
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
            device_->device(), swapChain_, &swapchainImageCount, nullptr );
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
        const auto err =
            vkGetSwapchainImagesKHR( device_->device(), swapChain_, &imageCount,
                                     swapChainImages_.data() );
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

        const auto err = vkCreateImageView( device_->device(), &createInfo,
                                            nullptr, &swapChainImageViews_[i] );
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

        const auto err = vkCreateFramebuffer(
            device_->device(), &framebufferInfo, nullptr, &framebuffers_[i] );
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
        if ( vkCreateSemaphore( device_->device(), &semaphoreInfo, nullptr,
                                &imageAvailableSemaphores_[i] ) != VK_SUCCESS ||
             vkCreateSemaphore( device_->device(), &semaphoreInfo, nullptr,
                                &renderFinishedSemaphores_[i] ) != VK_SUCCESS ||
             vkCreateFence( device_->device(), &fenceInfo, nullptr,
                            &inFlightFences_[i] ) != VK_SUCCESS ) {
            throw std::runtime_error( "failed to create semaphores!" );
        }
    }
}

}  // namespace tire
