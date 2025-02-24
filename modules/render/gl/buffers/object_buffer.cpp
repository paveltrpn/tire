
#include <cstring>

#include "object_buffer.h"
#include "render/gl/functions.h"

namespace tire::gl {

void ObjectBuffer::generate() {
    glGenVertexArrays( 1, &vertexArray_ );
    glGenBuffers( 1, &vertexBuffer_ );
    glGenBuffers( 1, &normalBuffer_ );
    glGenBuffers( 1, &texcrdBuffer_ );
}

void ObjectBuffer::clean() {
    glDeleteVertexArrays( 1, &vertexArray_ );
    glDeleteBuffers( 1, &vertexBuffer_ );
    glDeleteBuffers( 1, &normalBuffer_ );
    glDeleteBuffers( 1, &texcrdBuffer_ );
}

void ObjectBuffer::startBinding() {
    glBindVertexArray( vertexArray_ );
}

void ObjectBuffer::bindVertexData( long size, const void *data ) {
    glEnableVertexAttribArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer_ );
    glBufferData( GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

    verteciesCount_ = size;
}

void ObjectBuffer::bindNormalData( long size, const void *data ) {
    glEnableVertexAttribArray( 1 );
    glBindBuffer( GL_ARRAY_BUFFER, normalBuffer_ );
    glBufferData( GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
}

void ObjectBuffer::bindTexcrdData( long size, const void *data ) {
    glEnableVertexAttribArray( 2 );
    glBindBuffer( GL_ARRAY_BUFFER, texcrdBuffer_ );
    glBufferData( GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW );
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, nullptr );
}

void ObjectBuffer::updateVertexData( long size, const void *data ) {
    glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer_ );
    void *ptr = glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
    memcpy( ptr, data, size );
    glUnmapBuffer( GL_ARRAY_BUFFER );
}

void ObjectBuffer::updateNormalsData( long size, const void *data ) {
    glBindBuffer( GL_ARRAY_BUFFER, normalBuffer_ );
    void *ptr = glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
    memcpy( ptr, data, size );
    glUnmapBuffer( GL_ARRAY_BUFFER );
}

void ObjectBuffer::draw() {
    glBindVertexArray( vertexArray_ );
    glDrawArrays( GL_TRIANGLES, 0, verteciesCount_ );
    glBindVertexArray( 0 );
}

}  // namespace tire::gl
