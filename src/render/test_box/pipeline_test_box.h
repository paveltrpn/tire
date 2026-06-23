#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#include "context/context.h"
#include "context/pipeline.h"

namespace tire {

struct PiplineTestBox final : Pipeline {
    PiplineTestBox( const Context *context );

    auto buildPipeline( const Program &program ) -> void override;

    VkPipelineCache pipelineCache_{ VK_NULL_HANDLE };
};

}  // namespace tire
