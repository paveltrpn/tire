

#include <vector>

#include "pipeline.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_PIPELINE_CPP{ true };

namespace tire::vk {

Pipeline::~Pipeline() {
    vkDestroyPipelineLayout( device_, pipelineLayout, nullptr );
}

void PiplineSimple::initFixed() {
    // VkPipelineVertexInputStateCreateInfo
    vertexInput_.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInput_.vertexBindingDescriptionCount = 0;
    vertexInput_.pVertexBindingDescriptions = nullptr;  // Optional
    vertexInput_.vertexAttributeDescriptionCount = 0;
    vertexInput_.pVertexAttributeDescriptions = nullptr;

    // VkPipelineInputAssemblyStateCreateInfo
    inputAssembly_.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly_.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly_.primitiveRestartEnable = VK_FALSE;

    // VkViewport
    viewport_.x = 0.0f;
    viewport_.y = 0.0f;
    viewport_.width = static_cast<float>( 800 );   // swapChainExtent.width;
    viewport_.height = static_cast<float>( 600 );  // swapChainExtent.height;
    viewport_.minDepth = 0.0f;
    viewport_.maxDepth = 1.0f;

    // VkRect2D
    scissor_.offset = { 0, 0 };
    scissor_.extent = { 800, 600 };  //swapChainExtent;

    // VkPipelineViewportStateCreateInfo
    viewportState_.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState_.viewportCount = 1;
    viewportState_.pViewports = &viewport_;
    viewportState_.scissorCount = 1;
    viewportState_.pScissors = &scissor_;

    // VkPipelineRasterizationStateCreateInfo
    rasterizer_.sType =
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer_.depthClampEnable = VK_FALSE;
    rasterizer_.rasterizerDiscardEnable = VK_FALSE;
    rasterizer_.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer_.lineWidth = 1.0f;
    rasterizer_.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer_.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer_.depthBiasEnable = VK_FALSE;
    rasterizer_.depthBiasConstantFactor = 0.0f;  // Optional
    rasterizer_.depthBiasClamp = 0.0f;           // Optional
    rasterizer_.depthBiasSlopeFactor = 0.0f;     // Optional

    // VkPipelineMultisampleStateCreateInfo
    multisampling_.sType =
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling_.sampleShadingEnable = VK_FALSE;
    multisampling_.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling_.minSampleShading = 1.0f;           // Optional
    multisampling_.pSampleMask = nullptr;             // Optional
    multisampling_.alphaToCoverageEnable = VK_FALSE;  // Optional
    multisampling_.alphaToOneEnable = VK_FALSE;       // Optional

    // VkPipelineColorBlendAttachmentState
    colorBlendAttachment_.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment_.blendEnable = VK_FALSE;
    colorBlendAttachment_.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment_.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment_.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment_.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment_.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment_.alphaBlendOp = VK_BLEND_OP_ADD;  // Optional
    colorBlendAttachment_.blendEnable = VK_TRUE;
    colorBlendAttachment_.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment_.dstColorBlendFactor =
        VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment_.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment_.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment_.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment_.alphaBlendOp = VK_BLEND_OP_ADD;

    // VkPipelineColorBlendStateCreateInfo
    colorBlending_.sType =
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending_.logicOpEnable = VK_FALSE;
    colorBlending_.logicOp = VK_LOGIC_OP_COPY;  // Optional
    colorBlending_.attachmentCount = 1;
    colorBlending_.pAttachments = &colorBlendAttachment_;
    colorBlending_.blendConstants[0] = 0.0f;  // Optional
    colorBlending_.blendConstants[1] = 0.0f;  // Optional
    colorBlending_.blendConstants[2] = 0.0f;  // Optional
    colorBlending_.blendConstants[3] = 0.0f;  // Optional

    // VkPipelineDynamicStateCreateInfo
    std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT,
                                                  VK_DYNAMIC_STATE_LINE_WIDTH };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState_.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState_.dynamicStateCount =
        static_cast<uint32_t>( dynamicStates.size() );
    dynamicState_.pDynamicStates = dynamicStates.data();
}

void PiplineSimple::initProgable( VkShaderModule vert, VkShaderModule frag ) {
    // VkPipelineShaderStageCreateInfo
    vertShaderStage_.sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStage_.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStage_.module = vert;
    vertShaderStage_.pName = "main";

    // VkPipelineShaderStageCreateInfo
    fragShaderStage_.sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStage_.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStage_.module = frag;
    fragShaderStage_.pName = "main";
}

void PiplineSimple::initLayout() {
    // VkPipelineLayoutCreateInfo
    pipelineLayoutInfo_.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo_.setLayoutCount = 0;             // Optional
    pipelineLayoutInfo_.pSetLayouts = nullptr;          // Optional
    pipelineLayoutInfo_.pushConstantRangeCount = 0;     // Optional
    pipelineLayoutInfo_.pPushConstantRanges = nullptr;  // Optional

    const auto err = vkCreatePipelineLayout( device_, &pipelineLayoutInfo_,
                                             nullptr, &pipelineLayout );
    if ( err != VK_SUCCESS ) {
        throw std::runtime_error( "failed to create pipeline layout!" );
    } else {
        log::debug<DEBUG_OUTPUT_PIPELINE_CPP>(
            "simple pipeline layout created!" );
    }
}

void PiplineSimple::initRenderPass( VkFormat swapChainImageFormat ) {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChainImageFormat;
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

    const auto err =
        vkCreateRenderPass( device_, &renderPassInfo, nullptr, &renderPass );
    if ( err != VK_SUCCESS ) {
        throw std::runtime_error( "failed to create render pass!" );
    } else {
        log::debug<DEBUG_OUTPUT_PIPELINE_CPP>( "simple render pass created!" );
    }
}

}  // namespace tire::vk
