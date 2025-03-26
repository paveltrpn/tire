
#pragma once

#include <print>
#include <string>
#include <vector>
#include <algorithm>
#include <expected>
#include <variant>
#include <coroutine>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "context.h"
#include "pipelines/pipeline.h"
#include "buffers/vertex_buffer.h"
#include "command_pool.h"

#include "../render.h"
#include "scene.h"
#include "uv.h"

import event;
import io;
import config;

#define FRAMES_IN_FLIGHT_COUNT 2

namespace tire {

struct RenderVK final : Render {
    RenderVK();
    ~RenderVK() override = default;

    void displayRenderInfo() override {};
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
    event::Task<void> executeByTimeOut( long long timeout ) {
        co_await event::setTimeout( timeout );
        log::notice( "test coroutine timer trigger" );
    };

    event::Task<void> watchFile( std::string path ) {
        bool isRenamed{ false };
        while ( !isRenamed ) {
            const auto res = co_await io::watchForFile( path );
            // Wait for uv_fs_event_stop() to cancel next callback call
            co_await event::setTimeout( 100 );
            log::notice( "file  \"{}\" changed with code {}", path,
                         static_cast<int>( res ) );
            if ( isRenamed = ( res == UV_RENAME ); isRenamed ) {
                log::notice( "file  \"{}\" have been renamed, stop watching",
                             path );
            }
        }
    };

    event::Task<void> showMetrics() {
        while ( true ) {
            co_await event::setTimeout( 3000 );
            event::Context::metrics();
            log::info( "average frame duration = {}",
                       timer_.averageFrameDuration() );
            timer_.resetFrameMetrics();
        }
    }

private:
    std::unique_ptr<vk::Context> context_{};

    std::unique_ptr<vk::Pipeline> piplineMatrixReady_{};
    std::unique_ptr<vk::Pipeline> piplineVertexBuffer_{};

    uint32_t currentFrame_{ 0 };
};

}  // namespace tire
