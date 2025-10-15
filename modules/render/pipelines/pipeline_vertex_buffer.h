
#pragma once

#include "pipeline.h"

namespace tire::vk {

struct PiplineVertexBuffer final : Pipeline {
    PiplineVertexBuffer( const vk::Context* context )
        : Pipeline( context ) {}

    auto buildPipeline( const vk::Program& program ) -> void override;
};

}  // namespace tire::vk
