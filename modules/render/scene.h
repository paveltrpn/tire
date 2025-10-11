
#pragma once

#include <filesystem>
#include <memory>

#include <vulkan/vulkan.h>

#include "context.h"
#include "pipelines/pipeline.h"
#include "scene/scene.h"
#include "command_pool.h"
#include "commands/scene_render_command.h"
#include "buffers/vertex_buffer.h"
#include "vulkan/vulkan_core.h"

namespace tire::vk {

struct Scene final : tire::Scene {
    Scene( const std::filesystem::path& fname, const vk::Context* context,
           const vk::Pipeline* pipeline );

    void submit() override;
    void draw() override {};

    void clean() override {
        for ( auto& cbuf : cBufs_ ) {
            cbuf->clean();
        }

        for ( auto& buf : vertBuffersList_ ) {
            buf->clean();
        }

        for ( auto& buf : nrmlBuffersList_ ) {
            buf->clean();
        }
    };

    void output( const VkFramebuffer currentFramebuffer, uint32_t imageIndex,
                 VkSemaphore waitSemaphore, VkSemaphore signalSemaphore,
                 VkFence fence );

private:
    const vk::Context* context_;
    const vk::Pipeline* pipeline_;

    std::vector<std::unique_ptr<vk::SceneRenderCommand>> cBufs_{};
    std::vector<std::shared_ptr<vk::VertexBuffer<float>>> vertBuffersList_;
    std::vector<std::shared_ptr<vk::VertexBuffer<float>>> nrmlBuffersList_;
};

}  // namespace tire::vk
