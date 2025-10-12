
#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "pipelines/pipeline.h"
#include "buffers/vertex_buffer.h"
#include "command_pool.h"

#include "render.h"
#include "render/context.h"
#include "scene.h"

#define FRAMES_IN_FLIGHT_COUNT 2

namespace tire {

struct RenderVK final {
    RenderVK( vk::Context* context );
    ~RenderVK() = default;

    void displayRenderInfo() {};
    void setSwapInterval( int interval );

    void scene( const std::filesystem::path& path );
    void preLoop();
    void preFrame();
    void frame();
    void postFrame();
    void swapBuffers();
    void postLoop();

    void createGraphicsPipeline();
    void createSyncObjects();

private:
    vk::Context* context_{};

    std::unique_ptr<vk::Pipeline> piplineVertexBuffer_{};

    uint32_t currentFrame_{ 0 };

    // The Scene
    std::shared_ptr<Scene> scene_{};

    Timer timer_{};
};

}  // namespace tire
