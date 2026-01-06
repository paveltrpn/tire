
module;

#include <format>
#include <memory>

#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vk_enum_string_helper.h>

static constexpr bool DEBUG_OUTPUT_SWAPCHAIN_CPP{ true };

export module context:swapchain;

import config;
import log;

import :context;
import :depth_image;

namespace tire {

void Context::makeSwapchain() {
    const auto congigHandle = Config::instance();

    log::info(
      "vk::Swapchain === surface capabilities minImageCount: {}, "
      "maxImageCount: {}",
      surfaceCapabilities_.minImageCount, surfaceCapabilities_.maxImageCount );

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
    createInfo.imageExtent = { viewportWidth, viewportHeight };  //currentExtent_;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // const uint32_t graphicsFamily{ device_->graphicsFamily() };
    // const uint32_t presentFamily{ device_->presentFamily() };
    std::array<uint32_t, 2> queueFamilyIndices = { graphicsFamilyQueueId_, presentSupportQueueId_ };
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

    if ( const auto err = vkCreateSwapchainKHR( device_, &createInfo, nullptr, &swapchain_ ); err != VK_SUCCESS ) {
        log::fatal( "failed to create swapchain code {}\n!", string_VkResult( err ) );
    } else {
        log::info( "vk::Swapchain ===  vulkan swapchain created!" );
    }

    // Get swapchain images count. Sudden, this number is
    // equal to previously defined in VkSwapchainCreateInfoKHR.minImageCount (== framesCount_).
    // But we still try to get image count that way.
    if ( const auto err = vkGetSwapchainImagesKHR( device_, swapchain_, &swapchainImageCount_, nullptr );
         err != VK_SUCCESS ) {
        log::fatal( "failed to get swapchain images count with code {}\n!", string_VkResult( err ) );
    } else {
        log::debug<DEBUG_OUTPUT_SWAPCHAIN_CPP>( "vk::Swapchain === swapchain images count: {}", swapchainImageCount_ );
    }

    // Depth image
    depthImage_ = std::make_shared<DepthImage>( this, viewportWidth, viewportHeight );
}

void Context::makeFrames() {
    // Acquire all swapchain images at one call
    std::vector<VkImage> swapChainImages;
    swapChainImages.resize( framesCount_ );
    if ( const auto err = vkGetSwapchainImagesKHR( device_, swapchain_, &framesCount_, swapChainImages.data() );
         err != VK_SUCCESS ) {
        log::fatal( "failed to get swapchain images with code {}\n!", string_VkResult( err ) );
    } else {
        log::debug<DEBUG_OUTPUT_SWAPCHAIN_CPP>( "vk::Swapchain === images acquired!" );
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
          .components =
            { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
              VK_COMPONENT_SWIZZLE_IDENTITY },
          .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1 } };

        if ( const auto err = vkCreateImageView( device_, &createInfo, nullptr, &frames_[i].view_ );
             err != VK_SUCCESS ) {
            log::fatal( "failed to create swapchain image views with code {}\n!", string_VkResult( err ) );
        } else {
            log::debug<DEBUG_OUTPUT_SWAPCHAIN_CPP>( "vk::Swapchain === image view {} created!", i );
        }

        // Frame framebuffer
        std::array<VkImageView, 2> attachments = { frames_[i].view_, depthImage_->view() };
        const auto [viewportWidth, viewportHeight] = viewportSize();
        const VkFramebufferCreateInfo framebufferInfo{
          .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
          .renderPass = renderPass(),
          .attachmentCount = static_cast<uint32_t>( attachments.size() ),
          .pAttachments = attachments.data(),
          .width = viewportWidth,
          .height = viewportHeight,
          .layers = 1 };

        if ( const auto err = vkCreateFramebuffer( device_, &framebufferInfo, nullptr, &frames_[i].framebuffer_ );
             err != VK_SUCCESS ) {
            log::fatal( "failed to create framebuffer at {} with code {}!", i, string_VkResult( err ) );
        } else {
            log::debug<DEBUG_OUTPUT_SWAPCHAIN_CPP>( "vk::Swapchain === framebuffer {} created!", i );
        }

        // Frame synchronization primitieves
        VkSemaphoreCreateInfo semaphoreInfo{
          .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, .pNext = nullptr, .flags = 0 };

        VkFenceCreateInfo fenceInfo{
          .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, .pNext = nullptr, .flags = VK_FENCE_CREATE_SIGNALED_BIT };

        if (
          vkCreateSemaphore( device_, &semaphoreInfo, nullptr, &frames_[i].imageAvailableSemaphore_ ) != VK_SUCCESS ||
          vkCreateSemaphore( device_, &semaphoreInfo, nullptr, &frames_[i].renderFinishedSemaphore_ ) != VK_SUCCESS ||
          vkCreateFence( device_, &fenceInfo, nullptr, &frames_[i].inFlightFence_ ) != VK_SUCCESS ) {
            throw std::runtime_error( std::format( "failed to create semaphores!" ) );
        }

        const VkCommandBufferAllocateInfo allocInfo{
          .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
          .commandPool = commandPool(),
          .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
          .commandBufferCount = 1 };

        const auto err = vkAllocateCommandBuffers( device(), &allocInfo, &frames_[i].cbPrimary_ );
        if ( err != VK_SUCCESS ) {
            throw std::runtime_error(
              std::format( "failed to allocate command buffers with code {}!", string_VkResult( err ) ) );
        } else {
            log::debug<DEBUG_OUTPUT_SWAPCHAIN_CPP>( "vk::Context === primary command buffer created!" );
        };
    }
}

}  // namespace tire
