
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
            "no texture files corresponds \"{}\" material", setName_ ) );
    }

    for ( auto &&item : texturesList ) {
        if ( item.contains( colorMapSuffix ) ) {
            Tga image{ item };
            const auto color = static_cast<GLuint>( TextureType::COLOR );
            bindTextureSlot( set_[color], image );
            continue;
        }

        // Load normal map texture file.
        if ( item.contains( normalMapSuffix ) ) {
            Tga image{ item };
            const auto normal = static_cast<GLuint>( TextureType::NORMAL );
            bindTextureSlot( set_[normal], image );
            continue;
        }

        // Load roughness map texture file.
        if ( item.contains( roughnessMapSuffix ) ) {
            Tga image{ item };
            const auto roughness =
                static_cast<GLuint>( TextureType::ROUGHNESS );
            bindTextureSlot( set_[roughness], image );
            continue;
        }

        // Load displacement map texture file.
        if ( item.contains( displacementMapSuffix ) ) {
            Tga image{ item };
            const auto displacement =
                static_cast<GLuint>( TextureType::DISPLACEMENT );
            bindTextureSlot( set_[displacement], image );
            continue;
        }
    }
}

void TextureSet::clean() {
    glDeleteTextures( set_.size(), set_.data() );
}

void TextureSet::bindTextureSlot( GLuint slot, const Tga &image ) {
    glBindTexture( GL_TEXTURE_2D, slot );
    const auto width = image.widht();
    const auto height = image.height();
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                  GL_UNSIGNED_BYTE, image.data() );
    glGenerateMipmap( GL_TEXTURE_2D );

    // Maybe unbind?
    glBindTexture( GL_TEXTURE_2D, 0 );
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
    const auto color = static_cast<GLuint>( TextureType::COLOR );
    glBindTextureUnit( color, set_[color] );

    const auto normal = static_cast<GLuint>( TextureType::NORMAL );
    glBindTextureUnit( normal, set_[normal] );

    const auto roughness = static_cast<GLuint>( TextureType::ROUGHNESS );
    glBindTextureUnit( roughness, set_[roughness] );

    const auto displacement = static_cast<GLuint>( TextureType::DISPLACEMENT );
    glBindTextureUnit( displacement, set_[displacement] );
}

}  // namespace tire::gl