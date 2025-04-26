
#pragma once

#include <vector>
#include <string>
#include <array>
#include <GL/gl.h>

#include "image/tga.h"

namespace tire::gl {

enum class TextureType {
    COLOR = 0,
    NORMAL = 1,
    ROUGHNESS = 2,
    DISPLACEMENT = 3,
    AMBIENT_OCCLUSION = 4,
    METALNESS = 5,
};

struct TextureSet final {
    TextureSet() = default;

    TextureSet( const std::string &materialSetName );
    TextureSet( const TextureSet &other ) {
        setName_ = other.setName_;
        set_ = other.set_;
    }

    TextureSet( TextureSet &&other ) noexcept {
        setName_ = other.setName_;
        set_ = other.set_;

        other.setName_ = "";
        other.set_ = { 0, 0, 0, 0 };
    }

    TextureSet &operator=( const TextureSet &other ) {
        clean();

        setName_ = other.setName_;
        set_ = other.set_;

        return *this;
    }

    TextureSet &operator=( TextureSet &&other ) noexcept {
        clean();

        setName_ = other.setName_;
        set_ = other.set_;

        // Set other to default.
        other.setName_ = "";
        other.set_ = { 0, 0, 0, 0 };

        return *this;
    }

    ~TextureSet() { clean(); }

    void bind();

private:
    void init();
    void clean();
    void bindTextureSlot( TextureType slot, const Tga &image );

    // Return list of paths to texture files, that corresponds
    // given material name.
    std::vector<std::string> scanForTextureFiles();

private:
    std::string setName_{};

    // By now store four type of texures - color, normal map,
    // roughtness (used for specular) and displacement.
    std::array<GLuint, 4> set_{};
};

}  // namespace tire::gl