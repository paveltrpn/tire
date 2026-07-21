#pragma once

#include <coroutine>
#include <filesystem>
#include <format>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#include "context/context.h"
#include "config/config.h"
#include "log/log.h"

#include "timer.h"
#include "test_box/test_box.h"
#include "ui/ui.h"

#include "log/log.h"

// using namespace std::chrono_literals;

namespace tire {

struct RenderVK final {
    explicit RenderVK() {
        try {
            const auto basePath = Config::instance().getBasePath();

            ui_ = std::make_shared<UiVK>();

            testBox_ = std::make_shared<TestBox>();
            testBox_->setPosition( 1.0f, 0.0f, -4.0f );

            // RUN!!!
            run_ = true;
        } catch ( const std::runtime_error &e ) {
            log::fatal()( "error white creating render {}", e.what() );
        }
    }

    RenderVK( const RenderVK &other ) = delete;
    RenderVK( RenderVK &&other ) = delete;

    auto operator=( const RenderVK &other ) -> RenderVK & = delete;
    auto operator=( RenderVK &&other ) -> RenderVK & = delete;

    ~RenderVK() = default;

    [[nodiscard]] auto run() -> bool {
        //
        return run_;
    }

    auto displayRenderInfo() -> void{
        //
    };

    auto preLoop() -> void {
        //
        log::notice()( "render loop starts here..." );
    }

    auto frame() -> void {
        // Update global timer
        timer_.update();
        const auto duration = timer_.frameDuration<float>();

#define LABEL_POS_X -45.f
#define LABEL_POS_Y 45.0f
#define STRING_GAP 2.7f

        const auto frameDuration = std::format( "Frame duration: {}", duration );

        ui_->label( LABEL_POS_X, LABEL_POS_Y - STRING_GAP * 0.0f, "Test vulkan." );
        ui_->label( LABEL_POS_X, LABEL_POS_Y - STRING_GAP * 1.0f, "If you see this message" );
        ui_->label( LABEL_POS_X, LABEL_POS_Y - STRING_GAP * 2.0f, "then texture image is" );
        ui_->label( LABEL_POS_X, LABEL_POS_Y - STRING_GAP * 3.0f, "properly loaded. " );
        ui_->label( LABEL_POS_X, LABEL_POS_Y - STRING_GAP * 4.0f, frameDuration );

        ui_->billboard( LABEL_POS_X - 1.0f, LABEL_POS_Y + 1.0f, 32.0f, STRING_GAP * 6.0f, 0.0f );

        {
            auto cb = Context::instance().copyBufferCommand();
            ui_->upload( cb.buf() );
        }

        {
            auto cb = Context::instance().renderCommand( currentFrame_ );
            testBox_->draw( cb.buf(), timer_.floatFrameDuration() );
            ui_->draw( cb.buf() );
        }

        ui_->flush();

        currentFrame_ = ( currentFrame_ + 1 ) % Context::instance().framesCount();
    };

    auto postLoop() -> void {
        log::info()( "wait device idle..." );

        // We should wait for the logical device to finish operations
        // before exiting mainLoop and destroying the window.
        vkDeviceWaitIdle( Context::instance().device() );
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
    bool run_{};

    uint32_t currentFrame_{ 0 };

    // Ui.
    std::shared_ptr<UiVK> ui_{};

    // Test box.
    std::shared_ptr<TestBox> testBox_{};

    Timer timer_{};

    bool holdMouse_{ false };
};

}  // namespace tire
