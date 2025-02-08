
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

#include "instance.h"
#include "surface.h"
#include "device.h"
#include "swapchain.h"
#include "pipelines/pipeline.h"
#include "commands/commands.h"
#include "command_pool.h"
#include "present.h"

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
            isRenamed = ( res == UV_RENAME );
            // Wait for uv_fs_event_stop() to cancel next callback call
            co_await event::setTimeout( 100 );
            log::notice( "file  \"{}\" changed with code {}", path,
                         static_cast<int>( res ) );
        }
        log::notice( "file  \"{}\" have been renamed, stop watching", path );
    };

    event::Task<void> showLoopMerics() {
        while ( true ) {
            co_await event::setTimeout( 5000 );
            event::Context::metrics();
        }
    }

private:
    std::shared_ptr<vk::Scene> scene_;

    std::unique_ptr<vk::Instance> instance_{};
    std::unique_ptr<vk::Surface> surface_{};
    std::unique_ptr<vk::Device> device_{};
    std::unique_ptr<vk::Swapchain> swapchain_{};

    std::unique_ptr<vk::Pipeline> pipelineSimple_{};
    std::unique_ptr<vk::Pipeline> piplineMatrixReady{};

    std::unique_ptr<vk::CommandPool> commandPool_{};

    std::vector<std::unique_ptr<vk::RenderFromShader>> cBufs_{};
    std::unique_ptr<vk::DummyCommand> dummyCmd_{};

    std::unique_ptr<vk::Present> present_{};
    std::unique_ptr<vk::PresentSynchronization<FRAMES_IN_FLIGHT_COUNT>>
        presentSync_{};

    uint32_t currentFrame_{ 0 };
};

}  // namespace tire
