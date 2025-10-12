
#include <vulkan/vk_enum_string_helper.h>
#include <vector>

#include "pipeline.h"
#include "log/log.h"
#include "vulkan/vulkan_core.h"
static constexpr bool DEBUG_OUTPUT_PIPELINE_CPP{ true };

#include "config/config.h"

namespace tire::vk {

Pipeline::Pipeline( const vk::Context* context )
    : context_{ context } {
}

Pipeline::~Pipeline() {
    vkDestroyPipelineLayout( context_->device(), layout_, nullptr );
    vkDestroyRenderPass( context_->device(), renderPass_, nullptr );
    vkDestroyPipeline( context_->device(), pipeline_, nullptr );
}

void Pipeline::initShaderStages( const vk::Program& program ) {
    // Reserve space for all possible shader stages structs
    shaderStages_.reserve( 16 );

    // Add VERTEX stage
    if ( const auto module = program.get<ShaderStageType::VERTEX>();
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

    // Add FRAGMENT stage
    if ( const auto module = program.get<ShaderStageType::FRAGMENT>();
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

    // Add TESSEALTION_EVALUATION stage
    if ( const auto module = program.get<ShaderStageType::TESSELATION_EVAL>();
         module != VK_NULL_HANDLE ) {
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
    if ( const auto module = program.get<ShaderStageType::TESSELATION_CTRL>();
         module != VK_NULL_HANDLE ) {
        const VkPipelineShaderStageCreateInfo stage{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
            .module = module,
            .pName = "main" };
        shaderStages_.push_back( stage );
    }

    // Add GEOMETRY stage
    if ( const auto module = program.get<ShaderStageType::GEOMETRY>();
         module != VK_NULL_HANDLE ) {
        const VkPipelineShaderStageCreateInfo stage{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = VK_SHADER_STAGE_GEOMETRY_BIT,
            .module = module,
            .pName = "main" };
        shaderStages_.push_back( stage );
    }

    // Add MESH stage
    if ( const auto module = program.get<ShaderStageType::MESH>();
         module != VK_NULL_HANDLE ) {
        const VkPipelineShaderStageCreateInfo stage{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = VK_SHADER_STAGE_MESH_BIT_EXT,
            .module = module,
            .pName = "main" };
        shaderStages_.push_back( stage );
    }
}

}  // namespace tire::vk
