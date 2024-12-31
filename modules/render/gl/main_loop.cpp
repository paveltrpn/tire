
#include "rendergl.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_MAIN_LOOP_GL_CPP{ true };

namespace tire {

void RenderGL::preLoop() {
    auto program = gl::Shader{};
    program.link(
        { { GL_VERTEX_SHADER,
            shaderSourcesManager_.getVertexShader( "basic_color" ) },
          { GL_FRAGMENT_SHADER,
            shaderSourcesManager_.getFragmentShader( "basic_color" ) } } );

    program.use();

    auto matrix = program.getUniformLocation( "matrix" );

    program.setMatrixUniform( matrix, GL_FALSE,
                              scene_->getCamera( 0 )->getMatrix() );
    auto color = program.getUniformLocation( "color" );
    program.setVectorUniform( color, algebra::vector3f{ 0.9f, 0.2f, 0.5f } );

    log::debug<DEBUG_OUTPUT_MAIN_LOOP_GL_CPP>( "mat: {}, col: {}", matrix,
                                               color );

    programs_.insert( std::pair{ gl::ShaderID::BASIC_COLOR, program } );
    gl::Shader basic_color = programs_[gl::ShaderID::BASIC_COLOR];

    basic_color.use();
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
