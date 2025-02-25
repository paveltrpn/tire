
#include <iostream>
#include <string>
#include "algebra/matrix4.h"
#include "algebra/vector2.h"

#include "screen_string.h"
#include <GL/gl.h>
#include <GL/glcorearb.h>

namespace tire::gl {

ScreenString::ScreenString( const std::string& name )
    : ScreenStringBase( name ) {
    // glEnable( GL_TEXTURE_2D );
    // glGenTextures( 1, &font );
    // glBindTexture( GL_TEXTURE_2D, font );
    // glTexImage2D( GL_TEXTURE_2D, 0, 3, image.get_widht(), image.get_height(), 0,
    //   GL_RGB, GL_UNSIGNED_BYTE, image.get_data_ptr() );
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
    //  GL_LINEAR_MIPMAP_LINEAR );
    // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // glGenerateMipmap( GL_TEXTURE_2D );

    buffer_.generate();
    buffer_.startBinding();
    buffer_.bindVertexData( letterQuadsVertecies_.size(),
                            letterQuadsVertecies_.data() );
    buffer_.bindTexcrdData( letterQuadsTexcrds_.size(),
                            letterQuadsTexcrds_.data() );

    texture_.generate();
    texture_.bind();
    texture_.load( fontImage_->widht(), fontImage_->height(),
                   fontImage_->data() );

    shaderStorage_.add( "screen_string" );
};

void ScreenString::flush( algebra::matrix4f view ) {
    shaderStorage_.use( "screen_string" );
    shaderStorage_.setMatrixUniform( "screen_string", "view_matrix", GL_FALSE,
                                     view );

    buffer_.updateVertexData( letterQuadsVertecies_.size(),
                              letterQuadsVertecies_.data() );
    buffer_.updateTexcrdData( letterQuadsTexcrds_.size(),
                              letterQuadsTexcrds_.data() );
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR );
    texture_.bind();
    buffer_.draw();
    glDisable( GL_BLEND );
}

}  // namespace tire::gl
