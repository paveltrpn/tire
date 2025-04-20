
#include <GL/gl.h>

#include "texture.h"
#include "functions.h"

namespace tire::gl {

void Texture::generate() {
    glGenTextures( 1, &texture_ );
}

void Texture::clean() {
    glDeleteTextures( 1, &texture_ );
}

void Texture::bind( GLuint bpoint ) {
    glActiveTexture( GL_TEXTURE0 + bpoint );
    glBindTexture( GL_TEXTURE_2D, texture_ );
}

void Texture::load( GLuint width, GLuint height, const void *data ) {
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, data );
    glGenerateMipmap( GL_TEXTURE_2D );
}

}  // namespace tire::gl
