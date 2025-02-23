
#pragma once

#include <filesystem>
#include <memory>

#include <vulkan/vulkan.h>

#include "device.h"
#include "render/vk/pipelines/pipeline.h"
#include "scene/scene.h"
#include "command_pool.h"
#include "commands/scene_render_command.h"
#include "buffers/vertex_buffer.h"
#include "vulkan/vulkan_core.h"

namespace tire::vk {

struct Scene final : tire::Scene {
    Scene( const std::filesystem::path &fname, const vk::Device *device,
           const vk::Pipeline *pipeline );

    void submit();
    void output( const VkFramebuffer currentFramebuffer, uint32_t imageIndex,
                 VkSemaphore waitSemaphore, VkSemaphore signalSemaphore,
                 VkFence fence );

private:
    const vk::Device *device_;
    const vk::Pipeline *pipeline_;

    std::unique_ptr<vk::CommandPool> commandPool_{};
    std::vector<std::unique_ptr<vk::SceneRenderCommand>> cBufs_{};
    std::vector<std::shared_ptr<vk::VertexBuffer<float>>> buffersList_;
};

}  // namespace tire::vk
