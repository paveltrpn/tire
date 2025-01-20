
#include "renderpass.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_RENDERPASS_CPP{ true };

namespace tire::vk {

void RenderpassSimple::init() {
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

}  // namespace tire::vk
