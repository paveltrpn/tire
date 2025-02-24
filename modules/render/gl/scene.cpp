
#include "scene.h"
#include "functions.h"

import config;

namespace tire::gl {

Scene::Scene( const std::filesystem::path &fname )
    : tire::Scene{ fname } {
    const auto nodeListSize = nodeList_.size();
    buffersList_.reserve( nodeListSize );

    for ( size_t i{}; i < nodeListSize; ++i ) {
        SceneNodeBufferObjects buf{};

        glGenVertexArrays( 1, &buf.vertexArray );

        // Create object vertex buffer
        glGenBuffers( 1, &buf.vertexBuffer );
        glBindVertexArray( buf.vertexArray );
        glEnableVertexAttribArray( 0 );
        glBindBuffer( GL_ARRAY_BUFFER, buf.vertexBuffer );
        glBufferData( GL_ARRAY_BUFFER, nodeList_[i]->verteciesArraySize(),
                      nodeList_[i]->verteciesData(), GL_DYNAMIC_DRAW );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

        // Create object normal buffer
        glGenBuffers( 1, &buf.normalBuffer );
        glEnableVertexAttribArray( 1 );
        glBindBuffer( GL_ARRAY_BUFFER, buf.normalBuffer );
        glBufferData( GL_ARRAY_BUFFER, nodeList_[i]->normalsArraySize(),
                      nodeList_[i]->normalsData(), GL_DYNAMIC_DRAW );
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

        // Create object texture coordinates buffer
        glGenBuffers( 1, &buf.texcrdBuffer );
        glEnableVertexAttribArray( 2 );
        glBindBuffer( GL_ARRAY_BUFFER, buf.texcrdBuffer );
        glBufferData( GL_ARRAY_BUFFER, nodeList_[i]->texcrdsArraySize(),
                      nodeList_[i]->texcrdsData(), GL_STATIC_DRAW );
        glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, nullptr );

        // Create Texture object
        glGenTextures( 1, &buf.texture );
        glBindTexture( GL_TEXTURE_2D, buf.texture );

        const auto [width, height] = nodeList_[i]->textureSize();
        const auto data = nodeList_[i]->textureData();
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                         GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                      GL_UNSIGNED_BYTE, data );
        glGenerateMipmap( GL_TEXTURE_2D );

        // Store object buffers id's
        buffersList_.push_back( buf );
    }

    // Load shaders
    shaderStorage_.add( "flatshade" );
    shaderStorage_.use( "flatshade" );
    const Colorf lightColor = { 1.0f, 1.0f, 1.0f };
    shaderStorage_.setVectorUniform(
        "flatshade", "lightcolor",
        algebra::vector3f{ lightColor.r(), lightColor.g(), lightColor.b() } );

    shaderStorage_.setVectorUniform( "flatshade", "lightpos",
                                     algebra::vector3f{ 10.0f, 0.0f, 5.0f } );

    // ========================================================================

    shaderStorage_.add( "diffuse_texture" );
}

void Scene::submit() {
    for ( size_t i = 0; const auto &buffer : buffersList_ ) {
        {
            glBindBuffer( GL_ARRAY_BUFFER, buffer.vertexBuffer );
            void *ptr = glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
            memcpy( ptr, nodeList_[i]->verteciesData(),
                    nodeList_[i]->verteciesArraySize() );
            glUnmapBuffer( GL_ARRAY_BUFFER );
        }

        {
            glBindBuffer( GL_ARRAY_BUFFER, buffer.normalBuffer );
            void *ptr = glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );
            memcpy( ptr, nodeList_[i]->normalsData(),
                    nodeList_[i]->normalsArraySize() );
            glUnmapBuffer( GL_ARRAY_BUFFER );
        }
        ++i;
    }
}

void Scene::draw() {
    shaderStorage_.use( "diffuse_texture" );
    shaderStorage_.setMatrixUniform( "diffuse_texture", "view_matrix", GL_FALSE,
                                     getCamera( 0 )->matrix() );

    for ( size_t i = 0; const auto &buffer : buffersList_ ) {
        // const Colorf bodyColor = nodeList_[i]->color();
        // shaderStorage_.setVectorUniform(
        // "flatshade", "color",
        // algebra::vector3f{ bodyColor.r(), bodyColor.g(), bodyColor.b() } );

        glBindTexture( GL_TEXTURE_2D, buffer.texture );
        glBindVertexArray( buffer.vertexArray );
        glDrawArrays( GL_TRIANGLES, 0, nodeList_[i]->verteciesCount() );
        glBindVertexArray( 0 );

        ++i;
    }
}

}  // namespace tire::gl
