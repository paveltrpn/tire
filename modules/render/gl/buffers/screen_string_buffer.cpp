
#include <cstring>

#include "screen_string_buffer.h"
#include <GL/glext.h>
#include "render/gl/functions.h"

namespace tire::gl {

void ScreenStringBuffer::generate() {
    glGenVertexArrays( 1, &array_ );
    glGenBuffers( 3, buffers_.data() );
}

void ScreenStringBuffer::clean() {
    glDeleteVertexArrays( 1, &array_ );
    glDeleteBuffers( 3, buffers_.data() );
}

void ScreenStringBuffer::bind() {
    glBindVertexArray( array_ );
}

void ScreenStringBuffer::bindVertexData( long size, const void *data ) {
    glEnableVertexAttribArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, buffers_[0] );
    glBufferData( GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW );
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, nullptr );

    verteciesCount_ = size;
}

void ScreenStringBuffer::bindTexcrdData( long size, const void *data ) {
    glEnableVertexAttribArray( 2 );
    glBindBuffer( GL_ARRAY_BUFFER, buffers_[1] );
    glBufferData( GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW );
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, nullptr );
}

void ScreenStringBuffer::bindColorData( long size, const void *data ) {
    glEnableVertexAttribArray( 1 );
    glBindBuffer( GL_ARRAY_BUFFER, buffers_[2] );
    glBufferData( GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
}

void ScreenStringBuffer::updateVertexData( long size, const void *data ) {
    glBindBuffer( GL_ARRAY_BUFFER, buffers_[0] );
    void *ptr = glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
    memcpy( ptr, data, size );
    glUnmapBuffer( GL_ARRAY_BUFFER );
}

void ScreenStringBuffer::updateTexcrdData( long size, const void *data ) {
    glBindBuffer( GL_ARRAY_BUFFER, buffers_[1] );
    void *ptr = glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
    memcpy( ptr, data, size );
    glUnmapBuffer( GL_ARRAY_BUFFER );
}

void ScreenStringBuffer::updateColorData( long size, const void *data ) {
    glBindBuffer( GL_ARRAY_BUFFER, buffers_[2] );
    void *ptr = glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
    memcpy( ptr, data, size );
    glUnmapBuffer( GL_ARRAY_BUFFER );
}

void ScreenStringBuffer::release() {
    glBindVertexArray( 0 );
}

void ScreenStringBuffer::draw() {
    glBindVertexArray( array_ );
    glDrawArrays( GL_QUADS, 0, verteciesCount_ );
}

}  // namespace tire::gl
