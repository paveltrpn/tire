
#pragma once

#include "pipeline.h"

namespace tire::vk {

struct PiplineVertexBuffer final : Pipeline {
    PiplineVertexBuffer( const vk::Context* context )
        : Pipeline( context ) {}

    auto initShaderStages( const vk::Program& program ) -> void override;
    auto buildPipeline() -> void override;
};

}  // namespace tire::vk
