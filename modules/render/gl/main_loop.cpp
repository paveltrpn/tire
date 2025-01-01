
#include "rendergl.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_MAIN_LOOP_GL_CPP{ true };

namespace tire {

void RenderGL::preLoop() {
    const auto basePath = Config::instance()->getBasePath();
    gl::Program program{};

    program.initVertexStage( basePath / "assets" / "shaders" /
                             "test_vertex.glsl" );
    program.initFragmentStage( basePath / "assets" / "shaders" /
                               "test_fragment.glsl" );
    program.link();
    program.use();  // SIC!!!
    shaderStorage_.add( "main", program );
    shaderStorage_.use( "main" );

    auto matrix = shaderStorage_.getUniformLocation( "main", "matrix" );

    shaderStorage_.setMatrixUniform( matrix, GL_FALSE,
                                     scene_->getCamera( 0 )->getMatrix() );
    auto color = shaderStorage_.getUniformLocation( "main", "color" );
    shaderStorage_.setVectorUniform( color,
                                     algebra::vector3f{ 0.9f, 0.2f, 0.5f } );

    log::debug<DEBUG_OUTPUT_MAIN_LOOP_GL_CPP>( "mat: {}, col: {}", matrix,
                                               color );
};

void RenderGL::preFrame() {
    scene_->traverse();
}

void RenderGL::frame() {
    glViewport( 0, 0, width_, height_ );
    glClearColor( 0, 0.5, 1, 1 );
    glClear( GL_COLOR_BUFFER_BIT );

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );

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
