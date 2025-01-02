
#include "rendergl.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_MAIN_LOOP_GL_CPP{ true };

namespace tire {

void RenderGL::preLoop() {
    scene_->initPrograms();

    const auto backgroundColor = Colorf( "darksalmon" );
    glClearColor( backgroundColor.r(), backgroundColor.g(), backgroundColor.b(),
                  1 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );
};

void RenderGL::preFrame() {
    scene_->traverse();
}

void RenderGL::frame() {
    glViewport( 0, 0, width_, height_ );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    scene_->submit();
    scene_->output();
}

void RenderGL::postFrame() {
}

void RenderGL::swapBuffers() {
    glXSwapBuffers( display_, window_ );
}

void RenderGL::postLoop() {
}

}  // namespace tire
