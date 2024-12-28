
#pragma once

#include <vulkan/vulkan.h>

namespace tire::vk {

struct Pipeline {
    Pipeline( const VkDevice& device )
        : device_{ device } {}
    virtual ~Pipeline();

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
    VkPipelineLayoutCreateInfo pipelineLayoutInfo_{};
    VkPipelineRenderingCreateInfo renderInfo_;

    VkPipeline getPipeline() { return pipeline_; };
    VkRenderPass getRenderPass() { return renderPass_; }

protected:
    const VkDevice& device_;

    VkPipeline pipeline_{ VK_NULL_HANDLE };
    VkPipelineLayout pipelineLayout_{ VK_NULL_HANDLE };
    VkRenderPass renderPass_{ VK_NULL_HANDLE };
};

struct PiplineSimple final : Pipeline {
    PiplineSimple( const VkDevice& device )
        : Pipeline( device ) {}

    void initFixed();
    void initProgable( VkShaderModule vert, VkShaderModule frag );
    void initLayout();
    void initRenderPass( VkFormat swapChainImageFormat );
    void initPipeline();

private:
};

}  // namespace tire::vk
