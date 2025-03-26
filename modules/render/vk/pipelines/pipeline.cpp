
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

}  // namespace tire::vk
