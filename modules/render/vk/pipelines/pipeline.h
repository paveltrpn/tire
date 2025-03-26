
#pragma once

#include <vulkan/vulkan.h>

#include "../context.h"
#include "program.h"
#include "renderpass.h"

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
    [[nodiscard]] VkRenderPass renderpass() const {
        return renderpass_->handle();
    }

    virtual void initPipeline(
        const std::vector<std::filesystem::path> &files ) = 0;

protected:
    [[nodiscard]] virtual VkPipelineLayout initLayout() = 0;

protected:
    const vk::Context *context_;
    std::unique_ptr<vk::Program> program_;
    std::unique_ptr<vk::RenderpassBase> renderpass_;

    VkPipeline pipeline_{ VK_NULL_HANDLE };
    VkPipelineLayout layout_{ VK_NULL_HANDLE };
};

struct PiplineMatrixReady final : Pipeline {
    PiplineMatrixReady( const vk::Context *context )
        : Pipeline( context ) {}

public:
    void initPipeline(
        const std::vector<std::filesystem::path> &files ) override;

private:
    VkPipelineLayout initLayout() override;
};

// =====================================================================================

struct PiplineVertexBuffer final : Pipeline {
    PiplineVertexBuffer( const vk::Context *context )
        : Pipeline( context ) {}

public:
    void initPipeline(
        const std::vector<std::filesystem::path> &files ) override;

private:
    VkPipelineLayout initLayout() override;
};

}  // namespace tire::vk
