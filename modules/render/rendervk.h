
#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "pipelines/pipeline.h"
#include "buffers/vertex_buffer.h"

#include "timer.h"
#include "context.h"
#include "scene.h"

#define FRAMES_IN_FLIGHT_COUNT 2

#ifndef Q_MOC_RUN
import test_module;
#endif

namespace tire {

struct RenderVK final {
    RenderVK() = default;

    RenderVK( const RenderVK& other ) = delete;
    RenderVK( RenderVK&& other ) = delete;
    auto operator=( const RenderVK& other ) -> RenderVK& = delete;
    auto operator=( RenderVK&& other ) -> RenderVK& = delete;

    ~RenderVK() = default;

    auto init( vk::Context* context ) -> void;

    void displayRenderInfo() {};
    void setSwapInterval( int interval );

    void scene( const std::filesystem::path& path );
    void preLoop();
    void preFrame();
    void frame( VkCommandBuffer cb );
    void postFrame();
    void swapBuffers();
    void postLoop();

    void createGraphicsPipeline();
    void createSyncObjects();

public:
    void keyPressEvent( unsigned int key );
    void keyReleaseEvent( unsigned int key );
    void mouseButtonPressEvent( unsigned int key );
    void mouseButtonReleaseEvent( unsigned int key );

    // Call when mouse moves free upon window. "x" and "y"
    // represent current cursor position in window coordinates
    void mouseMoveEvent( unsigned int x, unsigned int y );

    // Call when mouse holds in defined position. "x" and "y"
    // represent current cursor ofssets.
    void mouseOffsetEvent( unsigned int x, unsigned int y );

private:
    auto drawTestCube( VkCommandBuffer cb ) -> void;

private:
    vk::Context* context_{};

    std::unique_ptr<vk::Pipeline> piplineVertexBuffer_{};
    std::unique_ptr<vk::Pipeline> piplineTestBox_{};

    // The Scene
    std::shared_ptr<Scene> scene_{};

    Timer timer_{};

    bool holdMouse_{ false };
    unsigned int holdMouseX_{ 500 };
    unsigned int holdMouseY_{ 500 };

    float angle_;

    PartitionOneObject* bar;
};

}  // namespace tire
