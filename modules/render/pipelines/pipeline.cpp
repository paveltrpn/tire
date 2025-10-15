
#include <vulkan/vk_enum_string_helper.h>
#include "vulkan/vulkan_core.h"

#include "pipeline.h"

namespace tire::vk {

Pipeline::Pipeline( const vk::Context* context )
    : context_{ context } {
}

Pipeline::~Pipeline() {
    vkDestroyPipelineLayout( context_->device(), layout_, nullptr );
    vkDestroyRenderPass( context_->device(), renderPass_, nullptr );
    vkDestroyPipeline( context_->device(), pipeline_, nullptr );
}

}  // namespace tire::vk
