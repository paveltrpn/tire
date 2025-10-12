
#pragma once

#include <filesystem>
#include <memory>

#include <vulkan/vulkan.h>

#include "context.h"
#include "pipelines/pipeline.h"
#include "scene/scene.h"
#include "buffers/vertex_buffer.h"
#include "vulkan/vulkan_core.h"

namespace tire::vk {

struct Scene final : tire::Scene {
    Scene( const std::filesystem::path& fname, const vk::Context* context,
           const vk::Pipeline* pipeline );

    void submit() override;
    void draw() override {};

    void clean() override {
        for ( auto& buf : vertBuffersList_ ) {
            buf->clean();
        }

        for ( auto& buf : nrmlBuffersList_ ) {
            buf->clean();
        }
    };

    void output( const VkCommandBuffer cb );

private:
    const vk::Context* context_;
    const vk::Pipeline* pipeline_;

    std::vector<std::shared_ptr<vk::VertexBuffer>> vertBuffersList_;
    std::vector<std::shared_ptr<vk::VertexBuffer>> nrmlBuffersList_;
};

}  // namespace tire::vk
