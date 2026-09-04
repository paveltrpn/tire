module;

#include <vector>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#include "context/context.h"

export module pipeline : pipeline;

import program;

namespace tire {

export struct Pipeline {
    Pipeline() = default;

    Pipeline( const Pipeline& other ) = delete;
    Pipeline( Pipeline&& other ) = delete;
    auto operator=( const Pipeline& other ) -> Pipeline& = delete;
    auto operator=( Pipeline&& other ) -> Pipeline& = delete;

    virtual ~Pipeline() {
        vkDestroyRenderPass( Context::instance().device(), renderPass_, nullptr );
        vkDestroyPipelineLayout( Context::instance().device(), layout_, nullptr );
        vkDestroyPipeline( Context::instance().device(), pipeline_, nullptr );
    }

    [[nodiscard]]
    auto pipeline() const -> VkPipeline {
        //
        return pipeline_;
    }

    [[nodiscard]] auto layout() const -> VkPipelineLayout {
        //
        return layout_;
    };

    [[nodiscard]] auto renderpass() const -> VkRenderPass {
        //
        return renderPass_;
    }

    virtual auto buildPipeline( const Program& program ) -> void = 0;

protected:
    VkPipeline pipeline_{ VK_NULL_HANDLE };
    VkPipelineLayout layout_{ VK_NULL_HANDLE };
    VkRenderPass renderPass_{ VK_NULL_HANDLE };

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages_{};
};

}  // namespace tire
