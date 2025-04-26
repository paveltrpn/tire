
#pragma once

#include <string>
#include <array>
#include <GL/gl.h>

namespace tuire::gl {

enum class TextureType {
    ALBEDO = 0,
    NORMAL,
    ROUGHNESS,
    DISPLACEMENT,
    AMBIENT_OCCLUSION,
    METALNESS
};

struct TextureSet final {
    void init( const std::string &materialSetName );
    void clean();

private:
    std::string setName_{};
    std::array<GLuint, 7> set_{};
};

}  // namespace tuire::gl