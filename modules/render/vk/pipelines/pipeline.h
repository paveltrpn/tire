
#pragma once

#include <vulkan/vulkan.h>

#include "../device.h"
#include "shader_storage.h"
#include "renderpass.h"

namespace tire::vk {

struct Pipeline {
    Pipeline( const vk::Device *device );

    Pipeline( const Pipeline &other ) = delete;
    Pipeline( Pipeline &&other ) = delete;
    Pipeline &operator=( const Pipeline &other ) = delete;
    Pipeline &operator=( Pipeline &&other ) = delete;

    virtual ~Pipeline();

    [[nodiscard]] VkPipeline pipeline() const { return pipeline_; };
    [[nodiscard]] VkRenderPass renderpass() const {
        return renderpass_->handle();
    }

    virtual void initFixed() = 0;
    virtual void initProgable(
        const std::vector<std::filesystem::path> &files ) = 0;
    virtual void initPipeline() = 0;

protected:
    [[nodiscard]] virtual VkPipelineLayout initLayout() = 0;

protected:
    const vk::Device *device_;
    std::unique_ptr<vk::ShaderStorage> shaderStorage_;
    std::unique_ptr<vk::RenderpassBase> renderpass_;

    // Vulkan entity handles
    VkPipeline pipeline_{ VK_NULL_HANDLE };

    // Vulkan related structures
    VkPipelineShaderStageCreateInfo vertShaderStage_{};
    VkPipelineShaderStageCreateInfo fragShaderStage_{};

    VkPipelineVertexInputStateCreateInfo vertexInput_{};
    VkPipelineInputAssemblyStateCreateInfo inputAssembly_{};
    VkFormat colorAttachmentformat_{};
    VkPipelineViewportStateCreateInfo viewportState_{};
    VkPipelineRasterizationStateCreateInfo rasterizer_{};
    VkPipelineMultisampleStateCreateInfo multisampling_{};
    VkPipelineDepthStencilStateCreateInfo depthAndStencil_{};
    VkPipelineColorBlendAttachmentState colorBlendAttachment_{};
    VkPipelineColorBlendStateCreateInfo colorBlending_{};
    VkPipelineDynamicStateCreateInfo dynamicState_{};

    VkPipelineRenderingCreateInfo renderInfo_;
};

struct PiplineSimple final : Pipeline {
    PiplineSimple( const vk::Device *device )
        : Pipeline( device ) {}

    void initFixed() override;
    void initProgable(
        const std::vector<std::filesystem::path> &files ) override;
    void initPipeline() override;

private:
    VkPipelineLayout initLayout() override;
};

}  // namespace tire::vk
