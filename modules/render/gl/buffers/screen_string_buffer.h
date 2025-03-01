
#pragma once

#include <array>
#include <GL/gl.h>

namespace tire::gl {

struct ScreenStringBuffer final {
    ScreenStringBuffer() = default;

    ScreenStringBuffer( const ScreenStringBuffer &other ) = delete;

    ScreenStringBuffer( ScreenStringBuffer &&other ) noexcept {
        this->verteciesCount_ = other.verteciesCount_;
        this->array_ = other.array_;
        this->buffers_ = other.buffers_;

        other.verteciesCount_ = 0;
        other.array_ = 0;
        other.buffers_[0] = 0;
        other.buffers_[1] = 0;
        other.buffers_[2] = 0;
    };

    ScreenStringBuffer &operator=( const ScreenStringBuffer &other ) = delete;

    ScreenStringBuffer &operator=( ScreenStringBuffer &&other ) noexcept {
        if ( &other != this ) {
            this->verteciesCount_ = other.verteciesCount_;
            this->array_ = other.array_;
            this->buffers_ = other.buffers_;

            other.verteciesCount_ = 0;
            other.array_ = 0;
            other.buffers_[0] = 0;
            other.buffers_[1] = 0;
            other.buffers_[2] = 0;
        }

        return *this;
    };

    ~ScreenStringBuffer() { clean(); };

    void generate();
    void clean();

    void bind();
    void bindVertexData( long size, const void *data );
    void bindTexcrdData( long size, const void *data );
    void bindColorData( long size, const void *data );

    void updateVertexData( long size, const void *data );
    void updateTexcrdData( long size, const void *data );
    void updateColorData( long size, const void *data );

    void release();

    void draw();

private:
    GLuint verteciesCount_{};

    GLuint array_{ 0 };
    std::array<GLuint, 3> buffers_{ 0 };
};

}  // namespace tire::gl
