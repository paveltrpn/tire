
#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <print>

#include <log/log.h>

#include "../device.h"

namespace tire::vk {

struct Renderpass {
    Renderpass( const vk::Device *device )
        : device_{ device } {}

    Renderpass( const Renderpass &other ) = delete;
    Renderpass( Renderpass &&other ) = delete;
    Renderpass &operator=( const Renderpass &other ) = delete;
    Renderpass &operator=( Renderpass &&other ) = delete;

    ~Renderpass() {
        vkDestroyRenderPass( device_->handle(), renderPass_, nullptr );
    };

    [[nodiscard]] VkRenderPass handle() const { return renderPass_; }

protected:
    const vk::Device *device_;
    VkRenderPass renderPass_{ VK_NULL_HANDLE };
};

struct RenderpassSimple : Renderpass {
    RenderpassSimple( const vk::Device *device )
        : Renderpass{ device } {
        VkAttachmentDescription colorAttachment{};
        // Same as swapchain image format
        colorAttachment.format = device_->surfaceFormat().format;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;

        if ( const auto err = vkCreateRenderPass(
                 device_->handle(), &renderPassInfo, nullptr, &renderPass_ );
             err != VK_SUCCESS ) {
            throw std::runtime_error(
                std::format( "failed to create render pass with code {}!",
                             string_VkResult( err ) ) );
        } else {
            log::info( "vk::RenderpassSimple === render pass created!" );
        }
    }
};

}  // namespace tire::vk
