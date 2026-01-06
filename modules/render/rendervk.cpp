
module;

#include <coroutine>
#include <filesystem>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

export module render:render;

import context;
import config;
import algebra;
import log;
import io;

import :timer;
import :scenevk;
import :test_box;

// using namespace std::chrono_literals;

namespace tire {

struct SceneVK;

export struct RenderVK final {
    RenderVK() = default;

    explicit RenderVK( Context *context )
        : context_{ context } {
        try {
            const auto configHandle = Config::instance();
            const auto basePath = configHandle->getBasePath();

            scene_ = std::make_shared<SceneVK>( basePath.string() + "/assets/m01.json", context_ );
            testBox_ = std::make_shared<TestBox>( context_ );

            // RUN!!!
            run_ = true;
        } catch ( const std::runtime_error &e ) {
            throw std::runtime_error( e.what() );
        }
    }

    RenderVK( const RenderVK &other ) = delete;
    RenderVK( RenderVK &&other ) = delete;

    auto operator=( const RenderVK &other ) -> RenderVK & = delete;
    auto operator=( RenderVK &&other ) -> RenderVK & = delete;

    ~RenderVK() = default;

    [[nodiscard]]
    auto run() -> bool {
        //
        return run_;
    }

    auto displayRenderInfo() -> void {
        //
    };

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
            testBox_->draw( cb.buf(), timer_.floatFrameDuration() );
        }

        currentFrame_ = ( currentFrame_ + 1 ) % context_->framesCount();
    };

    auto postLoop() -> void {
        log::info( "vk::Render === wait device idle..." );

        // We should wait for the logical device to finish operations
        // before exiting mainLoop and destroying the window.
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
    Context *context_{};
    bool run_{};

    uint32_t currentFrame_{ 0 };

    // Test box.
    std::shared_ptr<TestBox> testBox_{};

    // The Scene
    std::shared_ptr<SceneVK> scene_{};

    Timer timer_{};

    bool holdMouse_{ false };

    io::IoContext ioContext_{};
};

}  // namespace tire
