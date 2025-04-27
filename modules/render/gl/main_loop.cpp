
#include <GL/glcorearb.h>
#include "rendergl.h"
#include "log/log.h"
#include "scene/scene.h"
static constexpr bool DEBUG_OUTPUT_MAIN_LOOP_GL_CPP{ true };

import event;

namespace tire {

void RenderGL::preLoop() {
    // Start metrics logging coroutine.
    showMetrics();
};

void RenderGL::preFrame() {
    // Update global timer.
    timer_.update();

    // Update scene objects.
    scene_->traverse( timer_.frameDuration<float>() );

    scene_->camera().traverse();
}

void RenderGL::frame() {
    // Begin render frame.
    glViewport( 0, 0, width_, height_ );

    // Render to framebuffer.
    glBindFramebuffer( GL_FRAMEBUFFER, framebuffer_.frambufferObject_ );
    const auto bg = scene_->backgroundColor();
    glClearColor( bg.r(), bg.g(), bg.b(), 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Render scene.
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );

    scene_->submit();
    scene_->draw();

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glClearColor( bg.r(), bg.g(), bg.b(), 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    // Show framebuffer quad with frame texture.
    frameProgram_.use();
    glBindVertexArray( framebuffer_.array_ );
    glDisable( GL_DEPTH_TEST );
    glBindTexture( GL_TEXTURE_2D, framebuffer_.frameTexture_ );
    glDrawArrays( GL_QUADS, 0, 4 );

    // Render screen text upon the framebuffer quad.
    screenString_->setColor( { "white" } );
    screenString_->set_text_position( -31.5f, 31.5f );
    screenString_->draw(
        std::format( "fps = {}", 1000000 / timer_.averageFrameDuration() ) );

    const auto [xp, yp, zp] = scene_->camera().position();
    const auto cazimuth = scene_->camera().azimuth();
    const auto celevation = scene_->camera().elevation();

    screenString_->set_text_position( -31.5f, 29.5f );
    screenString_->setColor( { "red" } );
    screenString_->draw(
        std::format( "x:{:<7.2f} y:{:<7.2f} z:{:<7.2f} "
                     "azimuth:{:<3.2f} elevation:{:<3.2f}",
                     xp, yp, zp, cazimuth, celevation ) );

    screenString_->set_text_position( -31.5f, 27.5f );
    screenString_->setColor( { "white" } );
    screenString_->draw(
        std::format( "\"g\" - grub/ungrub cursor, \"q\" - next camera" ) );

    screenString_->set_text_position( -31.5f, 25.5f );
    screenString_->setColor( { "white" } );
    screenString_->draw(
        std::format( "active camera - {}", scene_->camera().name() ) );

    screenString_->flush();
    // End render frame.
}

void RenderGL::postFrame() {
}

void RenderGL::swapBuffers() {
    glXSwapBuffers( display_, window_ );
}

void RenderGL::postLoop() {
}

}  // namespace tire
