
module;

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

export module context:pipeline;

import :context;

import :program;

namespace tire {

export struct Pipeline {
    Pipeline( const Context *context )
        : context_{ context } {}

    Pipeline( const Pipeline &other ) = delete;
    Pipeline( Pipeline &&other ) = delete;
    auto operator=( const Pipeline &other ) -> Pipeline & = delete;
    auto operator=( Pipeline &&other ) -> Pipeline & = delete;

    virtual ~Pipeline() {
        vkDestroyRenderPass( context_->device(), renderPass_, nullptr );
        vkDestroyPipelineLayout( context_->device(), layout_, nullptr );
        vkDestroyPipeline( context_->device(), pipeline_, nullptr );
    }

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

    virtual auto buildPipeline( const Program &program ) -> void = 0;

protected:
    const Context *context_;

    VkPipeline pipeline_{ VK_NULL_HANDLE };
    VkPipelineLayout layout_{ VK_NULL_HANDLE };
    VkRenderPass renderPass_{ VK_NULL_HANDLE };

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages_{};
};

}  // namespace tire
