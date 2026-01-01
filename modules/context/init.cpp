
module;

#include <stdexcept>
#include <format>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include "vma/vk_mem_alloc.h"

export module context:init;

import config;
import log;
import image;

import :context;

namespace tire {

static void vkDestroyDebugUtilsMessenger(
  VkInstance instance, VkDebugUtilsMessengerEXT messanger, const VkAllocationCallbacks *pAllocator ) {
    auto func =
      (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr( instance, "vkDestroyDebugUtilsMessengerEXT" );
    if ( func != nullptr ) {
        return func( instance, messanger, pAllocator );
    }
}

auto Context::memoryRequirements( uint32_t typeFilter, VkMemoryPropertyFlags properties ) const -> uint32_t {
    VkPhysicalDeviceMemoryProperties memProperties{};
    vkGetPhysicalDeviceMemoryProperties( physDevice_, &memProperties );

    for ( uint32_t i = 0; i < memProperties.memoryTypeCount; i++ ) {
        if (
          ( typeFilter & ( 1 << i ) ) && ( memProperties.memoryTypes[i].propertyFlags & properties ) == properties ) {
            return i;
        }
    }

    log::fatal( "failed to find suitable memory type!" );

    // Silence warning
    return {};
}

auto Context::findSupportedFormat(
  const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features ) const -> VkFormat {
    for ( VkFormat format : candidates ) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties( physDevice_, format, &props );

        if ( tiling == VK_IMAGE_TILING_LINEAR && ( props.linearTilingFeatures & features ) == features ) {
            return format;
        } else if ( tiling == VK_IMAGE_TILING_OPTIMAL && ( props.optimalTilingFeatures & features ) == features ) {
            return format;
        }
    }

    throw std::runtime_error( std::format( "failed to find supported format!" ) );

    // Silence warning
    return {};
}

Context::~Context() {
    vkDestroyImage( device_, depthImage_, nullptr );
    vkDestroyImageView( device_, depthImageView_, nullptr );
    vkFreeMemory( device_, depthImageMemory_, nullptr );

    for ( auto i = 0; i < framesCount_; i++ ) {
        vkDestroySemaphore( device_, frames_[i].imageAvailableSemaphore_, nullptr );
        vkDestroySemaphore( device_, frames_[i].renderFinishedSemaphore_, nullptr );
        vkDestroyFence( device_, frames_[i].inFlightFence_, nullptr );
        vkDestroyFramebuffer( device_, frames_[i].framebuffer_, nullptr );
        vkDestroyImageView( device_, frames_[i].view_, nullptr );
    }

    vkDestroyCommandPool( device_, commandPool_, nullptr );

    vkDestroySwapchainKHR( device_, swapchain_, nullptr );

    vkDestroyDevice( device_, nullptr );
    vkDestroySurfaceKHR( instance_, surface_, nullptr );

    vkDestroyDebugUtilsMessenger( instance_, debugMessenger_, nullptr );

    vmaDestroyAllocator( allocator_ );

    vkDestroyInstance( instance_, nullptr );
}

void Context::makeCommandPool() {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = graphicsFamilyQueueId_;

    if ( const auto err = vkCreateCommandPool( device_, &poolInfo, nullptr, &commandPool_ ); err != VK_SUCCESS ) {
        throw std::runtime_error(
          std::format( "failed to create command pool woth code {}!", string_VkResult( err ) ) );
    } else {
        log::debug( "CommandPool === command pool created!" );
    }
}

auto Context::initRenderPass() -> void {
    VkAttachmentDescription colorAttachment{};
    // Same as swapchain image format
    colorAttachment.format = surfaceFormat().format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    //Similar to swapchain creation depth format acquire!
    const auto depthFormat = findSupportedFormat(
      { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL,
      VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT );
    // const auto depthFormat = VK_FORMAT_D32_SFLOAT;

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = depthFormat;
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependency.dstStageMask =
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>( attachments.size() );
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if ( const auto err = vkCreateRenderPass( device(), &renderPassInfo, nullptr, &renderPass_ ); err != VK_SUCCESS ) {
        throw std::runtime_error( std::format( "failed to create render pass with code {}!", string_VkResult( err ) ) );
    } else {
        log::info( "PipelineVertexBuffer === render pass created!" );
    }
}

auto Context::createAllocator() -> void {
    VmaVulkanFunctions vulkanFunctions{};
    vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
    vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

    VmaAllocatorCreateInfo allocatorCreateInfo{};
    allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT;
    allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_3;
    allocatorCreateInfo.physicalDevice = physDevice_;
    allocatorCreateInfo.device = device_;
    allocatorCreateInfo.instance = instance_;
    allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

    {
        const auto res = vmaCreateAllocator( &allocatorCreateInfo, &allocator_ );
        if ( res != VK_SUCCESS ) {
            throw std::runtime_error(
              std::format( "failed to create allocator with code {}!", string_VkResult( res ) ) );
        } else {
            log::info( "Context === allocator created!" );
        }
    }
}

}  // namespace tire
