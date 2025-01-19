
#pragma once

#include <print>
#include <string>
#include <vector>
#include <algorithm>
#include <expected>
#include <variant>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "instance.h"
#include "surface.h"
#include "device.h"
#include "swapchain.h"
#include "pipelines/pipeline.h"
#include "commands/commands.h"
#include "renderpasses/renderpass.h"
#include "command_pool.h"
#include "present.h"

#include "config/config.h"
#include "../render.h"
#include "shader_storage.h"
#include "scene.h"

namespace tire {

struct RenderVK final : Render {
    RenderVK();
    ~RenderVK() override;

    void displayRenderInfo() override{};
    void setSwapInterval( int interval ) override;

private:
    void scene( const std::filesystem::path &path ) override;
    void preLoop() override;
    void preFrame() override;
    void frame() override;
    void postFrame() override;
    void swapBuffers() override;
    void postLoop() override;

    void createGraphicsPipeline();
    void createSyncObjects();

private:
    std::shared_ptr<vk::Scene> scene_;

    std::unique_ptr<vk::Instance> instance_{};
    std::unique_ptr<vk::Surface> surface_{};
    std::unique_ptr<vk::Device> device_{};
    std::unique_ptr<vk::Swapchain> swapchain_{};
    std::unique_ptr<vk::ShaderStorage> shaderStorage_{};
    std::unique_ptr<vk::RenderpassSimple> renderpass_{};
    std::unique_ptr<vk::PiplineSimple> pipelineSimple_{};
    std::unique_ptr<vk::CommandPool> commandPool_{};
    std::vector<std::unique_ptr<vk::CommandBuffer>> cBufs_{};
    std::unique_ptr<vk::Present> present_{};

    std::vector<VkSemaphore> imageAvailableSemaphores_{};
    std::vector<VkSemaphore> renderFinishedSemaphores_{};
    std::vector<VkFence> inFlightFences_{};

    uint32_t currentFrame_{ 0 };
};

}  // namespace tire
