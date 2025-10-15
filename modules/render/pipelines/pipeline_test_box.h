
#pragma once

#include "pipeline.h"

namespace tire::vk {

struct PiplineTestBox final : Pipeline {
    PiplineTestBox( const vk::Context* context )
        : Pipeline( context ) {}

    auto buildPipeline( const vk::Program& program ) -> void override;

    VkPipelineCache pipelineCache_{ VK_NULL_HANDLE };
};

}  // namespace tire::vk
