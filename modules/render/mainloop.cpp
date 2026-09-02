
#include <format>

#include "rendervk.h"
#include "log/log.h"

import program;

namespace tire {

RenderVK::RenderVK() {
    try {
        _ui = std::make_shared<UiVK>();

        _testBox = std::make_shared<TestBox>();
        _testBox->setPosition( 0.0f, 0.0f, -2.5f );

        _run = true;
    } catch ( const std::runtime_error& e ) {
        log::fatal()( "error white creating render {}", e.what() );
    }
}

auto RenderVK::isRun() -> bool {
    //
    return _run;
}

auto RenderVK::displayRenderInfo() -> void {
    //

};

auto RenderVK::preLoop() -> void {
    //
    log::notice()( "render loop starts here..." );
}

auto RenderVK::frame() -> void {
    // Update global timer
    _timer.update();

#define INFO_LABEL_POS_X -42.f
#define INFO_LABEL_POS_Y 38.0f

#define STRING_GAP 3.2f

    const auto duration = _timer.frameDuration<float>();
    const auto frameDuration = std::format( "Frame duration: {}", duration );

    _ui->label( INFO_LABEL_POS_X, INFO_LABEL_POS_Y - STRING_GAP * 0.0f, "Test vulkan." );
    _ui->label( INFO_LABEL_POS_X, INFO_LABEL_POS_Y - STRING_GAP * 1.0f, "If you see this message" );
    _ui->label( INFO_LABEL_POS_X, INFO_LABEL_POS_Y - STRING_GAP * 2.0f, "then texture image is" );
    _ui->label( INFO_LABEL_POS_X, INFO_LABEL_POS_Y - STRING_GAP * 3.0f, "properly loaded. " );
    _ui->label( INFO_LABEL_POS_X, INFO_LABEL_POS_Y - STRING_GAP * 4.0f, frameDuration );

    _ui->billboard( INFO_LABEL_POS_X - 1.5f, INFO_LABEL_POS_Y + 1.5f, 31.0f, STRING_GAP * 6.0f, 0.0f );

#define CUBEPARAM_LABEL_POS_X 12.f
#define CUBEPARAM_LABEL_POS_Y 38.0f

    _ui->label( CUBEPARAM_LABEL_POS_X, CUBEPARAM_LABEL_POS_Y - STRING_GAP * 0.0f, "(TODO) To adjust cube" );
    _ui->label( CUBEPARAM_LABEL_POS_X, CUBEPARAM_LABEL_POS_Y - STRING_GAP * 1.0f, "appearence press:" );
    _ui->label( CUBEPARAM_LABEL_POS_X, CUBEPARAM_LABEL_POS_Y - STRING_GAP * 2.0f, "\"n\" - next appearence" );
    _ui->label( CUBEPARAM_LABEL_POS_X, CUBEPARAM_LABEL_POS_Y - STRING_GAP * 3.0f, "\"p\" - previous appearence" );

    _ui->billboard( CUBEPARAM_LABEL_POS_X - 1.5f, CUBEPARAM_LABEL_POS_Y + 1.5f, 30.0f, STRING_GAP * 5.0f, 0.0f );

    {
        auto cb = Context::instance().copyBufferCommand();
        _ui->upload( cb.buf() );
    }

    {
        auto cb = Context::instance().renderCommand( _currentFrame );
        _testBox->draw( cb.buf(), _timer.floatFrameDuration() );
        _ui->draw( cb.buf() );
    }

    _ui->flush();

    _currentFrame = ( _currentFrame + 1 ) % Context::instance().framesCount();
};

auto RenderVK::postLoop() -> void {
    log::info()( "wait device idle..." );

    // We should wait for the logical device to finish operations
    // before exiting mainLoop and destroying the window.
    vkDeviceWaitIdle( Context::instance().device() );
};

}  // namespace tire
