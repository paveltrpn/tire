
#include <vulkan/vk_enum_string_helper.h>
#include <vector>

#include "pipeline.h"
#include "log/log.h"
#include "vulkan/vulkan_core.h"
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
    // Add vertex stage
    if ( const auto module = program->get<ShaderStageType::VERTEX>();
         module != VK_NULL_HANDLE ) {
        const VkPipelineShaderStageCreateInfo stage{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .module = module,
            .pName = "main" };
        shaderStages_.push_back( stage );
    }

    // Add vertex stage
    if ( const auto module = program->get<ShaderStageType::FRAGMENT>();
         module != VK_NULL_HANDLE ) {
        const VkPipelineShaderStageCreateInfo stage{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = module,
            .pName = "main" };
        shaderStages_.push_back( stage );
    }
}

}  // namespace tire::vk
