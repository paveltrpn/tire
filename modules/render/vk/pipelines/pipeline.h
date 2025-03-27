
#pragma once

#include <vulkan/vulkan.h>

#include "../context.h"
#include "program.h"

namespace tire::vk {

struct Pipeline {
    Pipeline( const vk::Context *context );

    Pipeline( const Pipeline &other ) = delete;
    Pipeline( Pipeline &&other ) = delete;
    Pipeline &operator=( const Pipeline &other ) = delete;
    Pipeline &operator=( Pipeline &&other ) = delete;

    virtual ~Pipeline();

    [[nodiscard]] VkPipeline pipeline() const { return pipeline_; };
    [[nodiscard]] VkPipelineLayout layout() const { return layout_; };
    [[nodiscard]] VkRenderPass renderpass() const { return renderPass_; }

    virtual void initPipeline() = 0;

    void initShaderStages( const vk::Program *program );

protected:
    // Each pipeline can have unique layout
    [[nodiscard]] virtual VkPipelineLayout initLayout() = 0;
    // Each pipeline can have unique renderpass
    [[nodiscard]] virtual VkRenderPass initRenderpass() = 0;

protected:
    const vk::Context *context_;

    VkPipeline pipeline_{ VK_NULL_HANDLE };
    VkPipelineLayout layout_{ VK_NULL_HANDLE };
    VkRenderPass renderPass_{ VK_NULL_HANDLE };

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages_{};
};

struct PiplineMatrixReady final : Pipeline {
    PiplineMatrixReady( const vk::Context *context )
        : Pipeline( context ) {}

public:
    void initPipeline() override;

private:
    VkPipelineLayout initLayout() override;
    VkRenderPass initRenderpass() override;
};

// =====================================================================================

struct PiplineVertexBuffer final : Pipeline {
    PiplineVertexBuffer( const vk::Context *context )
        : Pipeline( context ) {}

public:
    void initPipeline() override;

private:
    VkPipelineLayout initLayout() override;
    VkRenderPass initRenderpass() override;
};

}  // namespace tire::vk
