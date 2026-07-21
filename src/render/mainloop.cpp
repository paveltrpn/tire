
#include <format>

#include "rendervk.h"

#include "log/log.h"

namespace tire {

RenderVK::RenderVK() {
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

auto RenderVK::isRun() -> bool {
    //
    return run_;
}

auto RenderVK::displayRenderInfo() -> void{
    //
};

auto RenderVK::preLoop() -> void {
    //
    log::notice()( "render loop starts here..." );
}

auto RenderVK::frame() -> void {
    // Update global timer
    timer_.update();

#define LABEL_POS_X -45.f
#define LABEL_POS_Y 45.0f
#define STRING_GAP 2.7f

    const auto duration = timer_.frameDuration<float>();
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

auto RenderVK::postLoop() -> void {
    log::info()( "wait device idle..." );

    // We should wait for the logical device to finish operations
    // before exiting mainLoop and destroying the window.
    vkDeviceWaitIdle( Context::instance().device() );
};

}  // namespace tire
