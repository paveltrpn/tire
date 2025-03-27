
#include <vulkan/vk_enum_string_helper.h>
#include <vector>

#include "pipeline.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_PIPELINE_CPP{ true };

import config;

namespace tire::vk {

Pipeline::Pipeline( const vk::Context *context )
    : context_{ context } {
}

Pipeline::~Pipeline() {
    vkDestroyPipeline( context_->device(), pipeline_, nullptr );
}

void Pipeline::initShaderStages( const vk::Program *program ) {
    const VkPipelineShaderStageCreateInfo vertShaderStage{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stage = VK_SHADER_STAGE_VERTEX_BIT,
        .module = program->get<ShaderStageType::VERTEX>(),
        .pName = "main" };

    const VkPipelineShaderStageCreateInfo fragShaderStage{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = program->get<ShaderStageType::FRAGMENT>(),
        .pName = "main" };

    shaderStages_.push_back( vertShaderStage );
    shaderStages_.push_back( fragShaderStage );
}

}  // namespace tire::vk
