
#pragma once

#include <GL/gl.h>

namespace tire::gl {

struct Texture final {
    Texture() = default;

    Texture( const Texture &other ) = delete;

    Texture( Texture &&other ) noexcept {
        this->texture_ = other.texture_;
        other.texture_ = 0;
    };

    Texture &operator=( const Texture &other ) = delete;

    Texture &operator=( Texture &&other ) noexcept {
        if ( &other != this ) {
            this->texture_ = other.texture_;
            other.texture_ = 0;
        }

        return *this;
    };

    ~Texture() { clean(); };

    void generate();
    void clean();

    void bind();
    void load( GLuint width, GLuint height, const void *data );

private:
    GLuint texture_{ 0 };
};

}  // namespace tire::gl
