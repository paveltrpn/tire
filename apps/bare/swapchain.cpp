
#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vk_enum_string_helper.h>

#include "context_bare.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_SWAPCHAIN_CPP{ true };

#include "config/config.h"
#include "context_bare.h"

namespace tire::vk {

namespace {

bool hasStencilComponent( VkFormat format ) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
           format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void transitionImageLayout( VkDevice device, VkCommandPool commandPool,
                            VkQueue graphicsQueue, VkImage image,
                            VkFormat format, VkImageLayout oldLayout,
                            VkImageLayout newLayout ) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers( device, &allocInfo, &commandBuffer );

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

    VkPipelineStageFlags sourceStage{};
    VkPipelineStageFlags destinationStage{};

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
        log::fatal( "unsupported layout transition!" );
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

    vkQueueSubmit( graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE );
    vkQueueWaitIdle( graphicsQueue );

    vkFreeCommandBuffers( device, commandPool, 1, &commandBuffer );
}

}  // namespace

void ContextBare::makeSwapchain() {
    const auto congigHandle = Config::instance();

    log::info(
        "vk::Swapchain === surface capabilities minImageCount: {}, "
        "maxImageCount: {}",
        surfaceCapabilities_.minImageCount,
        surfaceCapabilities_.maxImageCount );

    // NOTE: Warning from validation layers:
    // A Swapchain is being created with minImageCount set to 2, which means double buffering
    // is going to be used. Using double buffering and vsync locks rendering to an integer
    // fraction of the vsync rate. In turn, reducing the performance of the application if
    // rendering is slower than vsync. Consider setting minImageCount to 3 to use
    // triple buffering to maximize performance in such cases.
    // Skip all logic above, just use value from config
    framesCount_ = congigHandle->get<int>( "frame_count" );

    //if ( ( framesCount_ < surfaceCapabilities_.minImageCount ) ||
    //    ( framesCount_ > surfaceCapabilities_.maxImageCount ) ) {
    //  log::fatal(
    //      "vk::Swapchain === desired frame count not fit to available "
    //      "surface image count limits" );
    //}

    // Reserve space for frames images render into
    frames_.resize( framesCount_ );

    log::debug<DEBUG_OUTPUT_SWAPCHAIN_CPP>(
        "vk::Swapchain === vulkan swapchain surface capabilities image count "
        "set to "
        "{}",
        framesCount_ );

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface_;

    // The implementation will either create the swapchain with at least
    // that many images, or it will fail to create the swapchain.
    createInfo.minImageCount = framesCount_;

    // Use instead of extent ackuired from surface data.
    const auto [viewportWidth, viewportHeight] = viewportSize();

    createInfo.imageFormat = surfaceFormat_.format;
    createInfo.imageColorSpace = surfaceFormat_.colorSpace;
    createInfo.imageExtent = { viewportWidth,
                               viewportHeight };  //currentExtent_;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // const uint32_t graphicsFamily{ device_->graphicsFamily() };
    // const uint32_t presentFamily{ device_->presentFamily() };
    std::array<uint32_t, 2> queueFamilyIndices = { graphicsFamilyQueueId_,
                                                   presentSupportQueueId_ };
    if ( graphicsFamilyQueueId_ != presentSupportQueueId_ ) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;      // Optional
        createInfo.pQueueFamilyIndices = nullptr;  // Optional
    }

    createInfo.preTransform = surfaceCapabilities_.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode_;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if ( const auto err =
             vkCreateSwapchainKHR( device_, &createInfo, nullptr, &swapchain_ );
         err != VK_SUCCESS ) {
        log::fatal( "failed to create swapchain code {}\n!",
                    string_VkResult( err ) );
    } else {
        log::info( "vk::Swapchain ===  vulkan swapchain created!" );
    }

    // Get swapchain images count. Sudden, this number is
    // equal to previously defined in VkSwapchainCreateInfoKHR.minImageCount (== framesCount_).
    // But we still try to get image count that way.
    if ( const auto err = vkGetSwapchainImagesKHR(
             device_, swapchain_, &swapchainImageCount_, nullptr );
         err != VK_SUCCESS ) {
        log::fatal( "failed to get swapchain images count with code {}\n!",
                    string_VkResult( err ) );
    } else {
        log::debug<DEBUG_OUTPUT_SWAPCHAIN_CPP>(
            "vk::Swapchain === swapchain images count: {}",
            swapchainImageCount_ );
    }

    // Depth image
    auto createImage = [this]( uint32_t width, uint32_t height, VkFormat format,
                               VkImageTiling tiling, VkImageUsageFlags usage,
                               VkMemoryPropertyFlags properties, VkImage& image,
                               VkDeviceMemory& imageMemory ) -> void {
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

        if ( vkCreateImage( device_, &imageInfo, nullptr, &image ) !=
             VK_SUCCESS ) {
            log::fatal( "failed to create image!" );
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements( device_, image, &memRequirements );

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex =
            memoryRequirements( memRequirements.memoryTypeBits, properties );

        if ( vkAllocateMemory( device_, &allocInfo, nullptr, &imageMemory ) !=
             VK_SUCCESS ) {
            log::fatal( "failed to allocate image memory!" );
        }

        if ( vkBindImageMemory( device_, image, imageMemory, 0 ) !=
             VK_SUCCESS ) {
            log::fatal( "failed to bind image!" );
        }
    };

    const auto depthFormat = findSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
          VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT );
    //const auto depthFormat = VK_FORMAT_D32_SFLOAT;

    createImage(
        viewportWidth, viewportHeight, depthFormat, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
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

        VkImageView imageView{};
        if ( vkCreateImageView( device_, &viewInfo, nullptr, &imageView ) !=
             VK_SUCCESS ) {
            log::fatal( "failed to create image view!" );
        }

        return imageView;
    };

    depthImageView_ =
        createImageView( depthImage_, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT );

    transitionImageLayout( device_, commandPool_, graphicsQueue_, depthImage_,
                           depthFormat, VK_IMAGE_LAYOUT_UNDEFINED,
                           VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL );
}

void ContextBare::makeFrames() {
    // Acquire all swapchain images at one call
    std::vector<VkImage> swapChainImages;
    swapChainImages.resize( framesCount_ );
    if ( const auto err = vkGetSwapchainImagesKHR(
             device_, swapchain_, &framesCount_, swapChainImages.data() );
         err != VK_SUCCESS ) {
        log::fatal( "failed to get swapchain images with code {}\n!",
                    string_VkResult( err ) );
    } else {
        log::debug<DEBUG_OUTPUT_SWAPCHAIN_CPP>(
            "vk::Swapchain === images acquired!" );
    }

    // Create frame related vulkan entities - images, image views, framebuffers and sync primitieves.
    for ( size_t i{}; i < framesCount_; ++i ) {
        // Frame image
        frames_[i].image_ = swapChainImages[i];

        // Frame image view
        const VkImageViewCreateInfo createInfo{
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = swapChainImages[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = surfaceFormat_.format,
            .components = { VK_COMPONENT_SWIZZLE_IDENTITY,
                            VK_COMPONENT_SWIZZLE_IDENTITY,
                            VK_COMPONENT_SWIZZLE_IDENTITY,
                            VK_COMPONENT_SWIZZLE_IDENTITY },
            .subresourceRange = { .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                  .baseMipLevel = 0,
                                  .levelCount = 1,
                                  .baseArrayLayer = 0,
                                  .layerCount = 1 } };

        if ( const auto err = vkCreateImageView( device_, &createInfo, nullptr,
                                                 &frames_[i].view_ );
             err != VK_SUCCESS ) {
            log::fatal(
                "failed to create swapchain image views with code {}\n!",
                string_VkResult( err ) );
        } else {
            log::debug<DEBUG_OUTPUT_SWAPCHAIN_CPP>(
                "vk::Swapchain === image view {} created!", i );
        }

        // Frame framebuffer
        std::array<VkImageView, 2> attachments = { frames_[i].view_,
                                                   depthImageView_ };
        const auto [viewportWidth, viewportHeight] = viewportSize();
        const VkFramebufferCreateInfo framebufferInfo{
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = renderPass(),
            .attachmentCount = static_cast<uint32_t>( attachments.size() ),
            .pAttachments = attachments.data(),
            .width = viewportWidth,
            .height = viewportHeight,
            .layers = 1 };

        if ( const auto err = vkCreateFramebuffer(
                 device_, &framebufferInfo, nullptr, &frames_[i].framebuffer_ );
             err != VK_SUCCESS ) {
            log::fatal( "failed to create framebuffer at {} with code {}!", i,
                        string_VkResult( err ) );
        } else {
            log::debug<DEBUG_OUTPUT_SWAPCHAIN_CPP>(
                "vk::Swapchain === framebuffer {} created!", i );
        }

        // Frame synchronization primitieves
        VkSemaphoreCreateInfo semaphoreInfo{
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0 };

        VkFenceCreateInfo fenceInfo{
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_FENCE_CREATE_SIGNALED_BIT };

        if ( vkCreateSemaphore( device_, &semaphoreInfo, nullptr,
                                &frames_[i].imageAvailableSemaphore_ ) !=
                 VK_SUCCESS ||
             vkCreateSemaphore( device_, &semaphoreInfo, nullptr,
                                &frames_[i].renderFinishedSemaphore_ ) !=
                 VK_SUCCESS ||
             vkCreateFence( device_, &fenceInfo, nullptr,
                            &frames_[i].inFlightFence_ ) != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to create semaphores!" ) );
        }

        const VkCommandBufferAllocateInfo allocInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .commandPool = commandPool(),
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1 };

        const auto err = vkAllocateCommandBuffers( device(), &allocInfo,
                                                   &frames_[i].cbPrimary_ );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to allocate command buffers with code {}!",
                             string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_SWAPCHAIN_CPP>(
                "vk::Context === primary command buffer created!" );
        };
    }
}

}  // namespace tire::vk
