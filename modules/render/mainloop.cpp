
#include <format>

#include "context/context.h"
#include "rendervk.h"
#include "log/log.h"

import program;

namespace tire {

auto RenderVK::preLoop() -> void {
    if ( !_scene.get() ) {
        log::fatal()( "No scene attached!!!" );
    }

    log::notice()( "render loop starts here..." );
}

auto RenderVK::frame() -> void {
    auto& ctxHandle = Context::instance();

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
        auto cb = ctxHandle.copyBufferCommand();

        _ui->upload( cb.buf() );
    }

    {
        // Begin record render command buffer...

        auto cb = ctxHandle.renderCommand( _currentFrame );

        _scene->walk();
        _testBox->draw( cb.buf(), _timer.floatFrameDuration() );
        _ui->draw( cb.buf() );

        // ... end record render command buffer and submit.
    }

    _ui->flush();

    _currentFrame = ( _currentFrame + 1 ) % ctxHandle.framesCount();
};

auto RenderVK::postLoop() -> void {
    log::info()( "wait device idle..." );

    // We should wait for the logical device to finish operations
    // before exiting mainLoop and destroying the window.
    vkDeviceWaitIdle( Context::instance().device() );
};

}  // namespace tire
