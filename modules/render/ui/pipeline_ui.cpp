
module;

#include <format>
#include <cstring>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

export module render:pipeline_ui;

import log;
import algebra;
import context;

namespace tire {

struct PipelineUi final : Pipeline {
    PipelineUi( const Context *context )
        : Pipeline( context ) {}

#define SHADER_ENTRY_POINT "main"

    auto buildPipeline( const Program &program ) -> void override {
        // Add VERTEX stage
        if ( const auto module = program.get<ShaderStageType::VERTEX>(); module != VK_NULL_HANDLE ) {
            const VkPipelineShaderStageCreateInfo stage{
              .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
              .pNext = nullptr,
              .flags = 0,
              .stage = VK_SHADER_STAGE_VERTEX_BIT,
              .module = module,
              .pName = SHADER_ENTRY_POINT };
            shaderStages_.push_back( stage );
        }

        // Add FRAGMENT stage
        if ( const auto module = program.get<ShaderStageType::FRAGMENT>(); module != VK_NULL_HANDLE ) {
            const VkPipelineShaderStageCreateInfo stage{
              .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
              .pNext = nullptr,
              .flags = 0,
              .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
              .module = module,
              .pName = SHADER_ENTRY_POINT };
            shaderStages_.push_back( stage );
        }

        // Add TESSEALTION_EVALUATION stage
        if ( const auto module = program.get<ShaderStageType::TESSELATION_EVAL>(); module != VK_NULL_HANDLE ) {
            const VkPipelineShaderStageCreateInfo stage{
              .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
              .pNext = nullptr,
              .flags = 0,
              .stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
              .module = module,
              .pName = "main" };
            shaderStages_.push_back( stage );
        }

        // Add TESSELATION CONTROL stage
        if ( const auto module = program.get<ShaderStageType::TESSELATION_CTRL>(); module != VK_NULL_HANDLE ) {
            const VkPipelineShaderStageCreateInfo stage{
              .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
              .pNext = nullptr,
              .flags = 0,
              .stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
              .module = module,
              .pName = SHADER_ENTRY_POINT };
            shaderStages_.push_back( stage );
        }

        // Add GEOMETRY stage
        if ( const auto module = program.get<ShaderStageType::GEOMETRY>(); module != VK_NULL_HANDLE ) {
            const VkPipelineShaderStageCreateInfo stage{
              .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
              .pNext = nullptr,
              .flags = 0,
              .stage = VK_SHADER_STAGE_GEOMETRY_BIT,
              .module = module,
              .pName = SHADER_ENTRY_POINT };
            shaderStages_.push_back( stage );
        }

        // Add MESH stage
        if ( const auto module = program.get<ShaderStageType::MESH>(); module != VK_NULL_HANDLE ) {
            const VkPipelineShaderStageCreateInfo stage{
              .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
              .pNext = nullptr,
              .flags = 0,
              .stage = VK_SHADER_STAGE_MESH_BIT_EXT,
              .module = module,
              .pName = SHADER_ENTRY_POINT };
            shaderStages_.push_back( stage );
        }

        // Init fixed stages

        // NOTE: https://docs.vulkan.org/guide/latest/vertex_input_data_processing.html

        std::array<VkVertexInputBindingDescription, 3> bindingDescriptions{};

        // Prepare descriptors for VERTEX data
        bindingDescriptions[0].stride = sizeof( algebra::vector3f );
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        bindingDescriptions[0].binding = 0;

        // Prepare descriptors for NORMALS data
        bindingDescriptions[1].stride = sizeof( algebra::vector3f );
        bindingDescriptions[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        bindingDescriptions[1].binding = 1;

        // Prepare descriptors for UV data
        bindingDescriptions[2].stride = sizeof( algebra::vector2f );
        bindingDescriptions[2].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        bindingDescriptions[2].binding = 2;

        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        // Prepare descriptors for VERTEX data
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = 0;
        attributeDescriptions[0].location = 0;

        // Prepare descriptors for NORMALS data
        attributeDescriptions[1].binding = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = 0;
        attributeDescriptions[1].location = 1;

        // Prepare descriptors for UV data
        attributeDescriptions[2].binding = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = 0;
        attributeDescriptions[2].location = 2;

        const VkPipelineVertexInputStateCreateInfo vertexInput{
          .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
          .pNext = nullptr,
          .flags = 0,
          .vertexBindingDescriptionCount = bindingDescriptions.size(),
          .pVertexBindingDescriptions = bindingDescriptions.data(),
          .vertexAttributeDescriptionCount = static_cast<uint32_t>( attributeDescriptions.size() ),
          .pVertexAttributeDescriptions = attributeDescriptions.data() };

        const VkPipelineInputAssemblyStateCreateInfo inputAssembly{
          .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
          .pNext = nullptr,
          .flags = 0,
          .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
          .primitiveRestartEnable = VK_FALSE };

        const VkPipelineRasterizationStateCreateInfo rasterizer{
          .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
          .pNext = nullptr,
          .flags = 0,
          .depthClampEnable = VK_TRUE,
          .rasterizerDiscardEnable = VK_FALSE,
          .polygonMode = VK_POLYGON_MODE_FILL,
          .cullMode = VK_CULL_MODE_NONE,
          .frontFace = VK_FRONT_FACE_CLOCKWISE,
          .depthBiasEnable = VK_FALSE,
          .depthBiasConstantFactor = 0.0f,
          .depthBiasClamp = 0.0f,
          .depthBiasSlopeFactor = 0.0f,
          .lineWidth = 1.0f };

        const VkPipelineMultisampleStateCreateInfo multisampling{
          .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
          .pNext = nullptr,
          .flags = 0,
          .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
          .sampleShadingEnable = VK_FALSE,
          .minSampleShading = 1.0f,
          .pSampleMask = nullptr,
          .alphaToCoverageEnable = VK_FALSE,
          .alphaToOneEnable = VK_FALSE };

        const VkPipelineColorBlendAttachmentState colorBlendAttachment{
          .blendEnable = VK_FALSE,
          .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
          .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
          .colorBlendOp = VK_BLEND_OP_ADD,
          .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
          .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
          .alphaBlendOp = VK_BLEND_OP_ADD,
          .colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT };

        const VkPipelineColorBlendStateCreateInfo colorBlending{
          .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
          .pNext = nullptr,
          .flags = 0,
          .logicOpEnable = VK_FALSE,
          .logicOp = VK_LOGIC_OP_COPY,
          .attachmentCount = 1,
          .pAttachments = &colorBlendAttachment,
          .blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f } };

        const std::vector<VkDynamicState> dynamicStates = {
          VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_LINE_WIDTH };

        const VkPipelineDynamicStateCreateInfo dynamicState{
          .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
          .pNext = nullptr,
          .flags = 0,
          .dynamicStateCount = static_cast<uint32_t>( dynamicStates.size() ),
          .pDynamicStates = dynamicStates.data() };

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f;
        depthStencil.maxDepthBounds = 1.0f;
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {};
        depthStencil.back = {};

        // =============================================================================

        // This pipeline layout initialization.

        // Setup descriptor sets.
        // Texture sampler descriptor.
        const auto textureBind = VkDescriptorSetLayoutBinding{
          .binding = 0,
          .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
          .descriptorCount = 1,
          .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        };

        const auto textureDescSetCreateInfo = VkDescriptorSetLayoutCreateInfo{
          //
          .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
          .pNext = nullptr,
          .flags = 0,
          .bindingCount = 1,
          .pBindings = &textureBind,
        };

        vkCreateDescriptorSetLayout( context_->device(), &textureDescSetCreateInfo, nullptr, &textureDescSetLayout_ );

        // Omni lights data descriptor.
        const auto omniLightsBind = VkDescriptorSetLayoutBinding{
          .binding = 0,
          .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
          .descriptorCount = 1,
          .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
        };

        const auto omniLightsDescSetCreateInfo = VkDescriptorSetLayoutCreateInfo{
          //
          .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
          .pNext = nullptr,
          .flags = 0,
          .bindingCount = 1,
          .pBindings = &omniLightsBind,
        };

        vkCreateDescriptorSetLayout(
          context_->device(), &omniLightsDescSetCreateInfo, nullptr, &omniLightsDescSetLayout_ );

        std::array<VkDescriptorSetLayout, 2> texturedSetLayouts{ textureDescSetLayout_, omniLightsDescSetLayout_ };

        // Setup push constants.
        std::array<VkPushConstantRange, 1> constants{};

        constants[0] = VkPushConstantRange{
          .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
          .offset = 0,
          .size = sizeof( algebra::matrix4d ) + sizeof( algebra::vector3d ) };

        const auto pipelineLayoutInfo = VkPipelineLayoutCreateInfo{
          //
          .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
          .setLayoutCount = texturedSetLayouts.size(),
          .pSetLayouts = texturedSetLayouts.data(),
          .pushConstantRangeCount = constants.size(),
          .pPushConstantRanges = constants.data(),
        };

        if ( const auto err = vkCreatePipelineLayout( context_->device(), &pipelineLayoutInfo, nullptr, &layout_ );
             err != VK_SUCCESS ) {
            log::fatal()( "failed to create pipeline layout with code {}!", string_VkResult( err ) );
        }

        // Init render pass.
        renderPass_ = context_->renderPass();

        // Create pipeline
        const auto pipelineInfo = VkGraphicsPipelineCreateInfo{
          //
          .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
          .stageCount = static_cast<uint32_t>( shaderStages_.size() ),
          .pStages = shaderStages_.data(),
          .pVertexInputState = &vertexInput,
          .pInputAssemblyState = &inputAssembly,
          .pViewportState = &viewportState,
          .pRasterizationState = &rasterizer,
          .pMultisampleState = &multisampling,
          .pDepthStencilState = &depthStencil,
          .pColorBlendState = &colorBlending,
          .pDynamicState = &dynamicState,
          .layout = layout_,
          .renderPass = renderPass_,
          .subpass = 0,
          .basePipelineHandle = VK_NULL_HANDLE,
          .basePipelineIndex = -1,
        };

        if ( const auto err =
               vkCreateGraphicsPipelines( context_->device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline_ );
             err != VK_SUCCESS ) {
            log::fatal()( "failed to create graphics pipeline with code {}!", string_VkResult( err ) );
        }
    }

    [[nodiscard]]
    auto pipelineSescSetsLayout() const -> std::array<VkDescriptorSetLayout, 2> {
        //
        return { textureDescSetLayout_, omniLightsDescSetLayout_ };
    }

private:
    VkDescriptorSetLayout textureDescSetLayout_{};
    VkDescriptorSetLayout omniLightsDescSetLayout_{};
};

}  // namespace tire