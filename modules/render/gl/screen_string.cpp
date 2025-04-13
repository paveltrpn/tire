
#include <iostream>
#include <string>
#include <algorithm>

#include "algebra/matrix4.h"
#include "algebra/vector2.h"

#include "screen_string.h"
#include <GL/gl.h>
#include <GL/glcorearb.h>

namespace tire::gl {

ScreenString::ScreenString( const std::string& name )
    : ScreenStringBase( name ) {
    // Prepare vertex buffer
    buffer_.generate();
    buffer_.bind();
    buffer_.bindVertexData( letterQuadsVertecies_.size() * 4,
                            letterQuadsVertecies_.data() );
    buffer_.bindTexcrdData( letterQuadsTexcrds_.size() * 4,
                            letterQuadsTexcrds_.data() );
    buffer_.bindColorData( letterQuadsColors_.size() * 4,
                           letterQuadsColors_.data() );
    buffer_.release();

    // Prepare texture
    texture_.generate();
    texture_.bind();
    texture_.load( fontImage_->widht(), fontImage_->height(),
                   fontImage_->data() );

    // Prepare shader storage
    shaderStorage_.add( "screenString" );
};

void ScreenString::flush() {
    shaderStorage_.use( "screenString" );

    // Upload data
    buffer_.updateVertexData( letterQuadsVertecies_.size() * 4,
                              letterQuadsVertecies_.data() );
    buffer_.updateTexcrdData( letterQuadsTexcrds_.size() * 4,
                              letterQuadsTexcrds_.data() );
    buffer_.updateColorData( letterQuadsColors_.size() * 4,
                             letterQuadsColors_.data() );

    // After uploading vertex data to GPU
    // set CPU side buffers to default value.
    // May be use memset() instead? Ah, nah...
    std::ranges::fill( std::begin( letterQuadsVertecies_ ),
                       std::end( letterQuadsVertecies_ ),
                       algebra::vector2f{ 0.0f, 0.0f } );

    std::ranges::fill( std::begin( letterQuadsTexcrds_ ),
                       std::end( letterQuadsTexcrds_ ),
                       algebra::vector2f{ 0.0f, 0.0f } );

    const auto color = Colorf{ "white" }.asVector3f();
    std::ranges::fill( std::begin( letterQuadsColors_ ),
                       std::end( letterQuadsColors_ ), color );

    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR );
    texture_.bind();
    buffer_.draw();
    buffer_.release();
    glDisable( GL_BLEND );

    // Reset buffer position to begin write caracters from begining of the buffers
    bufferPos_ = 0;
}

}  // namespace tire::gl
