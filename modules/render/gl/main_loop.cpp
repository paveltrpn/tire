
#include "rendergl.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_MAIN_LOOP_GL_CPP{ true };

namespace tire {

void RenderGL::preLoop() {
    // Start metrics logging coroutine
    showMetrics();

    const auto backgroundColor = Colorf( "#0f0f0f" );
    glClearColor( backgroundColor.r(), backgroundColor.g(), backgroundColor.b(),
                  1.0f );
};

void RenderGL::preFrame() {
    // Update global timer
    timer_.update();
    // Update scene objects
    scene_->traverse( timer_.floatFrameDuration() );
}

void RenderGL::frame() {
    glViewport( 0, 0, width_, height_ );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );

    glDisable( GL_TEXTURE_2D );
    scene_->submit();
    scene_->draw();

    glDisable( GL_DEPTH_TEST );

    screenString_->setColor( { "white" } );
    screenString_->set_text_position( -31.5f, 31.5f );
    screenString_->draw(
        std::format( "fps = {}", 1000000 / timer_.averageFrameDuration() ) );

    const auto cpos = scene_->camera().position();
    const auto cazimuth = scene_->camera().azimuth();
    const auto celevation = scene_->camera().elevation();

    screenString_->set_text_position( -31.5f, 29.5f );
    screenString_->setColor( { "red" } );
    screenString_->draw(
        std::format( "x:{:<7.2f} y:{:<7.2f} z:{:<7.2f} "
                     "azimuth:{:<3.2f} elevation:{:<3.2f}",
                     cpos.x(), cpos.y(), cpos.z(), cazimuth, celevation ) );

    screenString_->set_text_position( -31.5f, 27.5f );
    screenString_->setColor( { "white" } );
    screenString_->draw( std::format( "press \"g\" for grub/ungrub cursor" ) );

    screenString_->flush();
}

void RenderGL::postFrame() {
}

void RenderGL::swapBuffers() {
    glXSwapBuffers( display_, window_ );
}

void RenderGL::postLoop() {
}

}  // namespace tire
