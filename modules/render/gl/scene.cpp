
#include "scene.h"
#include <GL/gl.h>
#include <GL/glcorearb.h>
#include "functions.h"
#include "render/gl/texture.h"
#include "scene/camera.h"

import config;

namespace tire::gl {

Scene::Scene( const std::filesystem::path &fname )
    : tire::Scene{ fname } {
    const auto nodeListSize = bodyList_.size();
    buffersList_.reserve( nodeListSize );

    for ( size_t i{}; i < nodeListSize; ++i ) {
        BodyBuffer buffer{};

        buffer.generate();
        buffer.bind();
        buffer.bindVertexData( bodyList_[i]->verteciesCount(),
                               bodyList_[i]->verteciesData() );
        buffer.bindNormalData( bodyList_[i]->normalsArraySize(),
                               bodyList_[i]->normalsData() );
        buffer.bindTexcrdData( bodyList_[i]->texcrdsArraySize(),
                               bodyList_[i]->texcrdsData() );
        buffer.release();
        // Store object buffers id's
        buffersList_.push_back( std::move( buffer ) );

        gl::Texture texture{};

        texture.generate();
        texture.bind();
        const auto &[tw, th] = bodyList_[i]->albedoTextureSize();
        texture.load( tw, th, bodyList_[i]->albedoTextureData() );

        texturesList_.push_back( std::move( texture ) );
    }

    // Load shaders
    shaderStorage_.add( "flatshade" );
    shaderStorage_.use( "flatshade" );
    const Colorf lightColorFlatshade = { 1.0f, 1.0f, 1.0f };
    shaderStorage_.setVectorUniform(
        "flatshade", "lightcolor",
        algebra::vector3f{ lightColorFlatshade.r(), lightColorFlatshade.g(),
                           lightColorFlatshade.b() } );

    shaderStorage_.setVectorUniform( "flatshade", "lightpos",
                                     algebra::vector3f{ 10.0f, 0.0f, 5.0f } );

    // ========================================================================

    shaderStorage_.add( "diffuseTexture" );

    // ========================================================================

    shaderStorage_.add( "flatshadeTexture" );
    shaderStorage_.use( "flatshadeTexture" );
    const Colorf lightColorShadedTex = { 1.0f, 1.0f, 1.0f };
    shaderStorage_.setVectorUniform(
        "flatshadeTexture", "lightcolor",
        algebra::vector3f{ lightColorShadedTex.r(), lightColorShadedTex.g(),
                           lightColorShadedTex.b() } );

    shaderStorage_.setVectorUniform( "flatshadeTexture", "lightpos",
                                     algebra::vector3f{ 10.0f, 0.0f, 5.0f } );
}

void Scene::submit() {
    for ( size_t i = 0; auto &buffer : buffersList_ ) {
        buffer.updateVertexData(
            bodyList_[i]->verteciesCount() * 3 * sizeof( float ),
            bodyList_[i]->verteciesData() );
        buffer.updateNormalsData(
            bodyList_[i]->verteciesCount() * 3 * sizeof( float ),
            bodyList_[i]->normalsData() );
        ++i;
    }
}

void Scene::draw() {
    shaderStorage_.use( "flatshadeTexture" );
    shaderStorage_.setMatrixUniform( "flatshadeTexture", "view_matrix",
                                     GL_FALSE,
                                     camera()->matrix<tire::OpenGLTag>() );

    for ( size_t i = 0; auto &buffer : buffersList_ ) {
        const Colorf bodyColor = bodyList_[i]->albedoColor();
        shaderStorage_.setVectorUniform(
            "flatshadeTexture", "color",
            algebra::vector3f{ bodyColor.r(), bodyColor.g(), bodyColor.b() } );

        texturesList_[i].bind();
        buffer.draw();
        buffer.release();
        ++i;
    }
}

}  // namespace tire::gl
