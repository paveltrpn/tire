
#include <filesystem>
#include <stdexcept>

#include "functions.h"
#include "material.h"
#include <GL/gl.h>
#include "image/tga.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_PROGRAM_CPP{ true };

import config;

namespace tire::gl {

// Texture files types suffixes.
const std::string colorMapSuffix = "color";
const std::string normalMapSuffix = "normalgl";
const std::string roughnessMapSuffix = "roughness";
const std::string displacementMapSuffix = "displacement";

TextureSet::TextureSet( const std::string &materialSetName )
    : setName_{ materialSetName } {
    glCreateTextures( GL_TEXTURE_2D, set_.size(), set_.data() );

    init();
}

void TextureSet::init() {
    const auto &texturesList = scanForTextureFiles();

    if ( texturesList.empty() ) {
        throw std::runtime_error( std::format(
            "no texture files corresponts \"{}\" material", setName_ ) );
    }

    for ( auto &&item : texturesList ) {
        // Load color map texture file.
        if ( item.contains( colorMapSuffix ) ) {
            Tga image{ item };
            bindTextureSlot( TextureType::COLOR, image );
            break;
        }

        // Load normal map texture file.
        if ( item.contains( normalMapSuffix ) ) {
            Tga image{ item };
            bindTextureSlot( TextureType::NORMAL, image );
            break;
        }

        // Load roughness map texture file.
        if ( item.contains( roughnessMapSuffix ) ) {
            Tga image{ item };
            bindTextureSlot( TextureType::ROUGHNESS, image );
            break;
        }

        // Load displacement map texture file.
        if ( item.contains( displacementMapSuffix ) ) {
            Tga image{ item };
            bindTextureSlot( TextureType::DISPLACEMENT, image );
            break;
        }
        log::info( "{}", item );
    }
}

void TextureSet::clean() {
    glDeleteTextures( set_.size(), set_.data() );
}

void TextureSet::bindTextureSlot( TextureType slot, const Tga &image ) {
}

std::vector<std::string> TextureSet::scanForTextureFiles() {
    // Find out texture files directory location
    const auto configHandle = Config::instance();
    const auto basePath = configHandle->getBasePath().string();
    const std::filesystem::path textureFilesPath =
        std::format( "{}/assets/textures/", basePath );

    std::vector<std::string> retItem;
    for ( auto &&entry :
          std::filesystem::directory_iterator{ textureFilesPath } ) {
        // Take only "name" part of filename, i.e. except
        // extension and path.
        const auto &fileName = entry.path().stem().string();

        // Check if file contains material texture file name template.
        // Name template is "{texture name}_{type}".
        if ( fileName.contains( setName_ ) ) {
            retItem.push_back( entry.path().string() );
        }
    }

    return retItem;
}

void TextureSet::bind() {
}

}  // namespace tire::gl