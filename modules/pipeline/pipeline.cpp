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
        vkDestroyRenderPass( Context::instance().device(), _renderPass, nullptr );
        vkDestroyPipelineLayout( Context::instance().device(), _layout, nullptr );
        vkDestroyPipeline( Context::instance().device(), _pipeline, nullptr );
    }

    [[nodiscard]]
    auto pipeline() const -> VkPipeline {
        //
        return _pipeline;
    }

    [[nodiscard]] auto layout() const -> VkPipelineLayout {
        //
        return _layout;
    };

    [[nodiscard]] auto renderpass() const -> VkRenderPass {
        //
        return _renderPass;
    }

    virtual auto buildPipeline( const Program& program ) -> void = 0;

protected:
    VkPipeline _pipeline{ VK_NULL_HANDLE };
    VkPipelineLayout _layout{ VK_NULL_HANDLE };
    VkRenderPass _renderPass{ VK_NULL_HANDLE };

    std::vector<VkPipelineShaderStageCreateInfo> _shaderStages{};
};

}  // namespace tire
