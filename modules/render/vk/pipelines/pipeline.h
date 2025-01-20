
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
    [[nodiscard]] VkPipelineLayout layout() const { return layout_; };
    [[nodiscard]] VkRenderPass renderpass() const {
        return renderpass_->handle();
    }

    virtual void initPipeline(
        const std::vector<std::filesystem::path> &files );

protected:
    [[nodiscard]] virtual VkPipelineLayout initLayout() = 0;

protected:
    const vk::Device *device_;
    std::unique_ptr<vk::ShaderStorage> shaderStorage_;
    std::unique_ptr<vk::RenderpassBase> renderpass_;

    VkPipeline pipeline_{ VK_NULL_HANDLE };
    VkPipelineLayout layout_{ VK_NULL_HANDLE };
};

// =====================================================================================

struct PiplineSimple final : Pipeline {
    PiplineSimple( const vk::Device *device )
        : Pipeline( device ) {}

private:
    VkPipelineLayout initLayout() override;
};

// =====================================================================================

struct PiplineMatrixReady final : Pipeline {
    PiplineMatrixReady( const vk::Device *device )
        : Pipeline( device ) {}

private:
    VkPipelineLayout initLayout() override;
};

}  // namespace tire::vk
