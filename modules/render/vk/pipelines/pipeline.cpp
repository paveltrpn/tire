
#include <vulkan/vk_enum_string_helper.h>
#include <vector>

#include "pipeline.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_PIPELINE_CPP{ true };

import config;

namespace tire::vk {

Pipeline::Pipeline( const vk::Device *device )
    : device_{ device } {
    shaderStorage_ = std::make_unique<vk::ShaderStorage>( device_ );
    renderpass_ = std::make_unique<vk::RenderpassSimple>( device_ );
}

Pipeline::~Pipeline() {
    vkDestroyPipeline( device_->handle(), pipeline_, nullptr );
}

}  // namespace tire::vk
