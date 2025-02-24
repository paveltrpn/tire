
#pragma once

#include <array>
#include <GL/gl.h>

namespace tire::gl {

struct ObjectBuffer final {
    ObjectBuffer() = default;

    ObjectBuffer( const ObjectBuffer &other ) = delete;

    ObjectBuffer( ObjectBuffer &&other ) {
        this->verteciesCount_ = other.verteciesCount_;
        this->vertexArray_ = other.vertexArray_;
        this->vertexBuffer_ = other.vertexBuffer_;
        this->normalBuffer_ = other.normalBuffer_;
        this->texcrdBuffer_ = other.texcrdBuffer_;

        other.verteciesCount_ = 0;
        other.vertexArray_ = 0;
        other.vertexBuffer_ = 0;
        other.normalBuffer_ = 0;
        other.texcrdBuffer_ = 0;
    };

    ObjectBuffer &operator=( const ObjectBuffer &other ) = delete;

    ObjectBuffer &operator=( ObjectBuffer &&other ) noexcept {
        if ( &other != this ) {
            this->vertexArray_ = other.vertexArray_;
            this->vertexBuffer_ = other.vertexBuffer_;
            this->normalBuffer_ = other.normalBuffer_;
            this->texcrdBuffer_ = other.texcrdBuffer_;

            other.vertexArray_ = 0;
            other.vertexBuffer_ = 0;
            other.normalBuffer_ = 0;
            other.texcrdBuffer_ = 0;
        }

        return *this;
    };

    ~ObjectBuffer() { clean(); };

    void generate();
    void clean();

    void startBinding();
    void bindVertexData( long size, const void *data );
    void bindNormalData( long size, const void *data );
    void bindTexcrdData( long size, const void *data );

    void updateVertexData( long size, const void *data );
    void updateNormalsData( long size, const void *data );

    void draw();

private:
    GLuint verteciesCount_{};

    // glDeleteBuffers() glDeleteVertexArrays() silently ignores 0's and names
    // that do not correspond to existing buffer objects.
    GLuint vertexArray_{ 0 };
    GLuint vertexBuffer_{ 0 };
    GLuint normalBuffer_{ 0 };
    GLuint texcrdBuffer_{ 0 };
};

}  // namespace tire::gl
