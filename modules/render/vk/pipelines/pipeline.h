
#pragma once

#include <vulkan/vulkan.h>

#include "../device.h"
#include "../shader_storage.h"

namespace tire::vk {

struct Pipeline {
    Pipeline( const vk::Device *device )
        : device_{ device } {}

    Pipeline( const Pipeline &other ) = delete;
    Pipeline( Pipeline &&other ) = delete;
    Pipeline &operator=( const Pipeline &other ) = delete;
    Pipeline &operator=( Pipeline &&other ) = delete;

    virtual ~Pipeline();

    [[nodiscard]] VkPipeline pipeline() const { return pipeline_; };

    [[nodiscard]] VkPipelineLayout pipelineLayout() const {
        return pipelineLayout_;
    };

    [[nodiscard]] VkRenderPass renderpass() const { return renderPass_; }

protected:
    const vk::Device *device_;
    const vk::ShaderStorage *shaderStorage_;

    // Vulkan entity handles
    VkPipeline pipeline_{ VK_NULL_HANDLE };
    VkPipelineLayout pipelineLayout_{ VK_NULL_HANDLE };
    VkRenderPass renderPass_{ VK_NULL_HANDLE };

    // Vulkan related structures
    VkPipelineShaderStageCreateInfo vertShaderStage_{};
    VkPipelineShaderStageCreateInfo fragShaderStage_{};
    VkPipelineShaderStageCreateInfo tessctrlShaderStage_{};
    VkPipelineShaderStageCreateInfo tessevalShaderStage_{};
    VkPipelineShaderStageCreateInfo geomShaderStage_{};
    VkPipelineShaderStageCreateInfo computeShaderStage_{};
    VkPipelineShaderStageCreateInfo raygenShaderStage_{};

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
};

struct PiplineSimple final : Pipeline {
    PiplineSimple( const vk::Device *device )
        : Pipeline( device ) {}

    void initFixed();
    void initProgable( VkShaderModule vert, VkShaderModule frag,
                       VkShaderModule tessctrl, VkShaderModule tesseval,
                       VkShaderModule geom, VkShaderModule compute,
                       VkShaderModule raygen );
    void initLayout();
    void initRenderPass();
    void initPipeline();
};

}  // namespace tire::vk
