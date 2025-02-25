
#pragma once

#include <iostream>
#include <string>
#include <array>

#include <GL/gl.h>

#include "render/screen_string_base.h"
#include "shader_storage.h"
#include "buffers/screen_string_buffer.h"
#include "texture.h"

namespace tire::gl {

struct ScreenString final : ScreenStringBase {
    ScreenString( const std::string& name );

    ScreenString( ScreenString& other ) = delete;
    ScreenString( ScreenString&& other ) = delete;
    ScreenString& operator=( ScreenString& other ) = delete;
    ScreenString& operator=( ScreenString&& other ) = delete;

    void flush( algebra::matrix4f view ) override;

private:
    ShaderStorage shaderStorage_;
    ScreenStringBuffer buffer_;
    Texture texture_;
};

}  // namespace tire::gl
