
#pragma once

#include <vulkan/vulkan.h>

#include "../context.h"
#include "program.h"

namespace tire::vk {

struct Pipeline {
    Pipeline( const vk::Context* context );

    Pipeline( const Pipeline& other ) = delete;
    Pipeline( Pipeline&& other ) = delete;
    auto operator=( const Pipeline& other ) -> Pipeline& = delete;
    auto operator=( Pipeline&& other ) -> Pipeline& = delete;

    virtual ~Pipeline();

    [[nodiscard]] auto pipeline() const -> VkPipeline {
        //
        return pipeline_;
    };

    [[nodiscard]] auto layout() const -> VkPipelineLayout {
        //
        return layout_;
    };

    [[nodiscard]] auto renderpass() const -> VkRenderPass {
        //
        return renderPass_;
    }

    virtual auto initShaderStages( const vk::Program& program ) -> void = 0;
    virtual auto buildPipeline() -> void = 0;

protected:
    const vk::Context* context_;

    VkPipeline pipeline_{ VK_NULL_HANDLE };
    VkPipelineLayout layout_{ VK_NULL_HANDLE };
    VkRenderPass renderPass_{ VK_NULL_HANDLE };

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages_{};
};

}  // namespace tire::vk
