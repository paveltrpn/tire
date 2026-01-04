
module;

// #include <thread>
// #include <chrono>
#include <coroutine>
#include <filesystem>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#define FRAMES_IN_FLIGHT_COUNT 2

export module render:render;

import context;
import config;
import algebra;
import log;
import io;

import :timer;
import :scenevk;
import :pipeline;
import :pipeline_test_box;
import :test_box_shader;
import :pipeline_vertex_buffer;

// using namespace std::chrono_literals;

namespace tire {

struct SceneVK;

export struct RenderVK final {
    RenderVK() = default;

    RenderVK( const RenderVK &other ) = delete;
    RenderVK( RenderVK &&other ) = delete;
    auto operator=( const RenderVK &other ) -> RenderVK & = delete;
    auto operator=( RenderVK &&other ) -> RenderVK & = delete;

    ~RenderVK() = default;

    auto init( Context *context ) -> void {
        context_ = context;
        try {
            const auto configHandle = Config::instance();
            const auto basePath = configHandle->getBasePath().string();

            // =============================================================
            piplineVertexBuffer_ = std::make_unique<PiplineVertexBuffer>( context_ );
            auto vertexBufferProgram = Program{ context_ };
            vertexBufferProgram.fill(
              { basePath + "/shaders/spirv/vk_vertexBuffer_VERTEX.spv",
                basePath + "/shaders/spirv/vk_vertexBuffer_FRAGMENT.spv" } );
            piplineVertexBuffer_->buildPipeline( vertexBufferProgram );

            // =============================================================
            piplineTestBox_ = std::make_unique<PiplineTestBox>( context_ );
            auto testBoxProgram = Program{ context_ };
            testBoxProgram.fill(
              { { vk_simple_box_VERTEX, vertex_stage_suffix }, { vk_simple_box_FRAGMENT, fragment_stage_suffix } } );
            piplineTestBox_->buildPipeline( testBoxProgram );

            // RUN!!!
            run_ = true;

            // context_->createFramebuffers( piplineVertexBuffer_.get() );
        } catch ( const std::runtime_error &e ) {
            throw std::runtime_error( e.what() );
        }
    }

    [[nodiscard]]
    auto run() -> bool {
        return run_;
    }

    auto displayRenderInfo() -> void {
        //
    };

    auto scene( const std::filesystem::path &path ) -> void {
        scene_ = std::make_shared<SceneVK>( path, context_, piplineVertexBuffer_.get() );
    }

    auto timeoutTestCoro( uint64_t t ) -> io::Task<void> {
        co_await ioContext_.timeout( t );
        log::info( "timeout {} exceeded once...", t );
        co_await ioContext_.timeout( t );
        log::info( "timeout {} exceeded again, coroutine done.", t );
    }

    auto preLoop() -> void {
        log::notice( "vk::Render === render loop starts here..." );

        timeoutTestCoro( 1000 );
        timeoutTestCoro( 2000 );
        timeoutTestCoro( 3000 );

        ioContext_.run();
    };

    auto frame() -> void {
        // Update global timer
        timer_.update();

        // Update scene objects
        scene_->traverse( timer_.frameDuration<float>() );

        scene_->camera().update();

        {
            auto cb = context_->copyBufferCommand();
            scene_->upload( cb.buf() );
        }

        {
            auto cb = context_->renderCommand( currentFrame_ );
            scene_->draw( cb.buf() );
        }

        // NOTE: About draw few geometry sets within same command buffer AI dummy said:
        // "Synchronization: If the rendering of the two geometry sets has dependencies
        // (e.g., one writes to a resource that the other reads), appropriate pipeline
        // barriers or synchronization primitives might be required. However,
        // for independent rendering to the same attachments within a single
        // render pass, Vulkan handles some synchronization implicitly.""

        // drawTestCube( cb );

        currentFrame_ = ( currentFrame_ + 1 ) % context_->framesCount();
    };

    auto postLoop() -> void {
        log::info( "vk::Render === wait device idle..." );

        // we should wait for the logical device to finish operations
        // before exiting mainLoop and destroying the window
        vkDeviceWaitIdle( context_->device() );

        scene_->clean();
    };

public:
    auto holdMouse() -> bool;

    auto keyPressEvent( unsigned int key ) -> void;

    auto keyReleaseEvent( unsigned int key ) -> void;

    auto mouseButtonPressEvent( unsigned int key ) -> void;

    auto mouseButtonReleaseEvent( unsigned int key ) -> void;

    // Call when mouse moves free upon window. "x" and "y"
    // represent current cursor position in window coordinates
    auto mouseMoveEvent( double x, double y ) -> void;

    // Call when mouse holds in defined position. "x" and "y"
    // represent current cursor ofssets.
    auto mouseOffsetEvent( double x, double y, double holdX, double holdY ) -> void;

private:
    auto drawTestCube( VkCommandBuffer cb ) -> void {
        // =================================
        // Get transformation matricies
        auto offset = algebra::translate( 0.0f, 0.0f, -2.0f );
        offset.transpose_self();

        const auto [width, height] = context_->currentExtent();
        // NOTE: Choose right projection matrix!!!
        const auto proj = algebra::perspective<float>(
          50.0f, static_cast<float>( width ) / static_cast<float>( height ), 0.1f, 100.0f );
        const auto viewMatrix = offset * proj;
        angle_ += timer_.floatFrameDuration() * 25.0f;
        algebra::vector3f ax{ 0.0f, 1.0f, 1.0f };
        ax.normalizeSelf();
        const auto modelMatrix = algebra::rotate( ax, angle_ );
        // =================================

        vkCmdBindPipeline( cb, VK_PIPELINE_BIND_POINT_GRAPHICS, piplineTestBox_->pipeline() );

        vkCmdPushConstants(
          cb, piplineTestBox_->layout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof( algebra::matrix4f ), &viewMatrix );

        vkCmdPushConstants(
          cb, piplineTestBox_->layout(), VK_SHADER_STAGE_VERTEX_BIT, sizeof( algebra::matrix4f ),
          sizeof( algebra::matrix4f ), &modelMatrix );

        vkCmdDraw( cb, 36, 3, 0, 0 );
    }

private:
    Context *context_{};
    bool run_{};

    uint32_t currentFrame_{ 0 };

    std::unique_ptr<Pipeline> piplineVertexBuffer_{};
    std::unique_ptr<Pipeline> piplineTestBox_{};

    // The Scene
    std::shared_ptr<SceneVK> scene_{};

    Timer timer_{};

    bool holdMouse_{ false };

    float angle_{};

    io::IoContext ioContext_{};
};

}  // namespace tire
