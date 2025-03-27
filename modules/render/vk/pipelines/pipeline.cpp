
#include <vulkan/vk_enum_string_helper.h>
#include <vector>

#include "pipeline.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_PIPELINE_CPP{ true };

import config;

namespace tire::vk {

Pipeline::Pipeline( const vk::Context *context )
    : context_{ context } {
    program_ = std::make_unique<vk::Program>( context_ );
    renderpass_ = std::make_unique<vk::RenderpassSimple>( context_ );
}

Pipeline::~Pipeline() {
    vkDestroyPipeline( context_->device(), pipeline_, nullptr );
}

void Pipeline::initShaderStages(
    const std::vector<std::filesystem::path> &files ) {
    program_->fill( files );

    const VkPipelineShaderStageCreateInfo vertShaderStage{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stage = VK_SHADER_STAGE_VERTEX_BIT,
        .module = program_->get<ShaderStageType::VERTEX>(),
        .pName = "main" };

    const VkPipelineShaderStageCreateInfo fragShaderStage{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = program_->get<ShaderStageType::FRAGMENT>(),
        .pName = "main" };

    shaderStages_.push_back( vertShaderStage );
    shaderStages_.push_back( fragShaderStage );
}

}  // namespace tire::vk
