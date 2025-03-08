
#include <array>

#include <vulkan/vk_enum_string_helper.h>

#include "rendervk.h"
#include "swapchain.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_SWAPCHAINVK_CPP{ true };

import config;

namespace tire::vk {
namespace {

bool hasStencilComponent( VkFormat format ) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
           format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void transitionImageLayout( const vk::Device *device,
                            const vk::CommandPool *commandPool, VkImage image,
                            VkFormat format, VkImageLayout oldLayout,
                            VkImageLayout newLayout ) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool->handle();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers( device->handle(), &allocInfo, &commandBuffer );

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer( commandBuffer, &beginInfo );

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if ( newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL ) {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        if ( hasStencilComponent( format ) ) {
            barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
    } else {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    if ( oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
         newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL ) {
        barrier.srcAccessMask = 0;

        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if ( oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
                newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;

        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else if ( oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
                newLayout ==
                    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL ) {
        barrier.srcAccessMask = 0;

        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
                                VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

        destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    } else {
        throw std::invalid_argument( "unsupported layout transition!" );
    }
    /*
    if ( oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
         newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL ) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if ( oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
                newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else {
        throw std::invalid_argument( "unsupported layout transition!" );
    }
   */
    vkCmdPipelineBarrier( commandBuffer, sourceStage, destinationStage, 0, 0,
                          nullptr, 0, nullptr, 1, &barrier );

    vkEndCommandBuffer( commandBuffer );

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit( device->graphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE );
    vkQueueWaitIdle( device->graphicsQueue() );

    vkFreeCommandBuffers( device->handle(), commandPool->handle(), 1,
                          &commandBuffer );
}
}  // namespace

Swapchain::Swapchain( const vk::Device *device, const vk::Surface *surface,
                      const vk::CommandPool *commandPool )
    : device_{ device }
    , surface_{ surface }
    , commandPool_{ commandPool } {
}

Swapchain::~Swapchain() {
    for ( auto framebuffer : framebuffers_ ) {
        vkDestroyFramebuffer( device_->handle(), framebuffer, nullptr );
    }
    vkDestroySwapchainKHR( device_->handle(), swapchain_, nullptr );
}

void Swapchain::createSwapchain() {
    swapChainExtent_ = device_->extent();

    const auto surfaceFormat = device_->surfaceFormat();
    const auto presentMode = device_->presentMode();

    const auto surfaceCapabilities = device_->surfaceCapabilities();

    // Choose count of images to draw in.
    uint32_t imageCount{};
    {
        // However, simply sticking to this minimum means that we may sometimes have
        // to wait on the driver to complete internal operations before we can acquire
        // another image to render to. Therefore it is recommended to request at least one
        // more image than the minimum:
        imageCount = surfaceCapabilities.minImageCount + 1;

        // We should also make sure to not exceed the maximum number of images while
        // doing this, where 0 is a special value that means that there is no maximum:
        if ( surfaceCapabilities.maxImageCount > 0 &&
             imageCount > surfaceCapabilities.maxImageCount ) {
            imageCount = surfaceCapabilities.maxImageCount;
        }

        // Skip all logic above, just use to two images
        imageCount = FRAMES_IN_FLIGHT_COUNT;
    }

    log::debug<DEBUG_OUTPUT_SWAPCHAINVK_CPP>(
        "vk::Swapchain: vulkan swapchain surface capabilities image count: {}, "
        "with max is {}",
        imageCount, surfaceCapabilities.maxImageCount );

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface_->handle();

    // The implementation will either create the swapchain with at least
    // that many images, or it will fail to create the swapchain.
    createInfo.minImageCount = imageCount;

    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = swapChainExtent_;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    const uint32_t graphicsFamily{ device_->graphicsFamily() };
    const uint32_t presentFamily{ device_->presentFamily() };
    std::array<uint32_t, 2> queueFamilyIndices = { graphicsFamily,
                                                   presentFamily };
    if ( graphicsFamily != presentFamily ) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
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

    if ( const auto err = vkCreateSwapchainKHR( device_->handle(), &createInfo,
                                                nullptr, &swapchain_ );
         err != VK_SUCCESS ) {
        throw std::runtime_error( std::format(
            "failed to create swapchain code {}\n!", string_VkResult( err ) ) );
    } else {
        log::info( "vk::Swapchain ===  vulkan swapchain created!" );
    }

    // Get swapchain images count. Sudden, this number is
    // equal to previously defined in VkSwapchainCreateInfoKHR.minImageCount.
    // But we still try to get image count that way.
    if ( const auto err = vkGetSwapchainImagesKHR(
             device_->handle(), swapchain_, &swapchainImageCount_, nullptr );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to get swapchain images count with code {}\n!",
                         string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_SWAPCHAINVK_CPP>(
            "vk::Swapchain === swapchain images count: {}",
            swapchainImageCount_ );
    }

    swapChainImages_.resize( imageCount );

    if ( const auto err =
             vkGetSwapchainImagesKHR( device_->handle(), swapchain_,
                                      &imageCount, swapChainImages_.data() );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to get swapchain images with code {}\n!",
                         string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_SWAPCHAINVK_CPP>(
            "vk::Swapchain === images acquired!" );
    }

    swapChainImageFormat_ = surfaceFormat.format;

    // Depth image cration
    auto createImage = [this]( uint32_t width, uint32_t height, VkFormat format,
                               VkImageTiling tiling, VkImageUsageFlags usage,
                               VkMemoryPropertyFlags properties, VkImage &image,
                               VkDeviceMemory &imageMemory ) {
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if ( vkCreateImage( device_->handle(), &imageInfo, nullptr, &image ) !=
             VK_SUCCESS ) {
            throw std::runtime_error( "failed to create image!" );
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements( device_->handle(), image,
                                      &memRequirements );

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = device_->memoryRequirements(
            memRequirements.memoryTypeBits, properties );

        if ( vkAllocateMemory( device_->handle(), &allocInfo, nullptr,
                               &imageMemory ) != VK_SUCCESS ) {
            throw std::runtime_error( "failed to allocate image memory!" );
        }

        vkBindImageMemory( device_->handle(), image, imageMemory, 0 );
    };

    const auto depthFormat = device_->findSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
          VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT );

    createImage(
        swapChainExtent_.width, swapChainExtent_.height, depthFormat,
        VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage_, depthImageMemory_ );

    auto createImageView = [this](
                               VkImage image, VkFormat format,
                               VkImageAspectFlags aspectFlags ) -> VkImageView {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if ( vkCreateImageView( device_->handle(), &viewInfo, nullptr,
                                &imageView ) != VK_SUCCESS ) {
            throw std::runtime_error( "failed to create image view!" );
        }

        return imageView;
    };

    depthImageView_ =
        createImageView( depthImage_, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT );

    transitionImageLayout( device_, commandPool_, depthImage_, depthFormat,
                           VK_IMAGE_LAYOUT_UNDEFINED,
                           VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL );
}

void Swapchain::createImageViews() {
    swapChainImageViews_.resize( swapChainImages_.size() );
    for ( size_t i = 0; i < swapChainImages_.size(); i++ ) {
        const VkImageViewCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = swapChainImages_[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = swapChainImageFormat_,
            .components = { VK_COMPONENT_SWIZZLE_IDENTITY,
                            VK_COMPONENT_SWIZZLE_IDENTITY,
                            VK_COMPONENT_SWIZZLE_IDENTITY,
                            VK_COMPONENT_SWIZZLE_IDENTITY },
            .subresourceRange = { .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                  .baseMipLevel = 0,
                                  .levelCount = 1,
                                  .baseArrayLayer = 0,
                                  .layerCount = 1 } };

        if ( const auto err =
                 vkCreateImageView( device_->handle(), &createInfo, nullptr,
                                    &swapChainImageViews_[i] );
             err != VK_SUCCESS ) {
            throw std::runtime_error( std::format(
                "failed to create swapchain image views with code {}\n!",
                string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_SWAPCHAINVK_CPP>(
                "vk::Swapchain === image view {} created!", i );
        }
    }
}

void Swapchain::createFramebuffers( const vk::Pipeline *pipeline ) {
    framebuffers_.resize( swapChainImageViews_.size() );
    for ( size_t i = 0; i < swapChainImageViews_.size(); i++ ) {
        std::array<VkImageView, 2> attachments = { swapChainImageViews_[i],
                                                   depthImageView_ };
        const VkFramebufferCreateInfo framebufferInfo{
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = pipeline->renderpass(),
            .attachmentCount = static_cast<uint32_t>( attachments.size() ),
            .pAttachments = attachments.data(),
            .width = swapChainExtent_.width,
            .height = swapChainExtent_.height,
            .layers = 1 };

        if ( const auto err =
                 vkCreateFramebuffer( device_->handle(), &framebufferInfo,
                                      nullptr, &framebuffers_[i] );
             err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to create framebuffer at {} with code {}!",
                             i, string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_SWAPCHAINVK_CPP>(
                "vk::Swapchain === framebuffer {} created!", i );
        }
    }
}

}  // namespace tire::vk
