
#pragma once

#include <vulkan/vulkan.h>

namespace tire::vk {

struct PipelineConfig final {
    VkPipelineShaderStageCreateInfo vertShaderStage_{};
    VkPipelineShaderStageCreateInfo fragShaderStage_{};
    VkPipelineVertexInputStateCreateInfo vertexInput_{};
    VkPipelineInputAssemblyStateCreateInfo inputAssembly_{};
    VkViewport viewport_{};
    VkRect2D scissor_{};
    VkFormat colorAttachmentformat_{};
    VkPipelineViewportStateCreateInfo viewportState_{};
    VkPipelineRasterizationStateCreateInfo rasterizer_{};
    VkPipelineMultisampleStateCreateInfo multisampling_{};
    VkPipelineDepthStencilStateCreateInfo depthAndStencil_{};
    VkPipelineColorBlendAttachmentState colorBlendAttachment_{};
    VkPipelineColorBlendStateCreateInfo colorBlending_{};
    VkPipelineDynamicStateCreateInfo dynamicState_{};

    VkPipelineLayout pipelineLayout{ VK_NULL_HANDLE };
    VkPipelineLayoutCreateInfo pipelineLayoutInfo_{};

    VkPipelineRenderingCreateInfo renderInfo_;
};

struct Pipeline final {
    Pipeline( const VkDevice& device )
        : device_{ device } {}

    ~Pipeline() {}

private:
    const VkDevice& device_;
    VkPipeline pipeline_{ VK_NULL_HANDLE };
};

}  // namespace tire::vk
