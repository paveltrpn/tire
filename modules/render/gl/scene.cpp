
#include "scene.h"
#include <GL/gl.h>
#include <GL/glcorearb.h>
#include "functions.h"
#include "render/gl/texture.h"

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
    shaderStorage_.add( "pplTexture" );
    shaderStorage_.use( "pplTexture" );

    shaderStorage_.setScalarUniform( "pplTexture", "lightsCount",
                                     int( lightList_.size() ) );

    for ( size_t i{ 0 }; i < lightList_.size(); ++i ) {
        const auto position = std::format( "omniLights[{}].position", i );
        shaderStorage_.setVectorUniform( "pplTexture", position.c_str(),
                                         lightList_[i]->position() );

        const auto constant = std::format( "omniLights[{}].constant", i );
        shaderStorage_.setScalarUniform( "pplTexture", constant.c_str(),
                                         lightList_[i]->constant() );

        const auto linear = std::format( "omniLights[{}].linear", i );
        shaderStorage_.setScalarUniform( "pplTexture", linear.c_str(),
                                         lightList_[i]->linear() );

        const auto quadratic = std::format( "omniLights[{}].quadratic", i );
        shaderStorage_.setScalarUniform( "pplTexture", quadratic.c_str(),
                                         lightList_[i]->quadratic() );

        const auto ambient = std::format( "omniLights[{}].ambient", i );
        shaderStorage_.setVectorUniform( "pplTexture", ambient.c_str(),
                                         lightList_[i]->ambient() );

        const auto diffuse = std::format( "omniLights[{}].diffuse", i );
        shaderStorage_.setVectorUniform( "pplTexture", diffuse.c_str(),
                                         lightList_[i]->diffuse() );

        const auto specular = std::format( "omniLights[{}].specular", i );
        shaderStorage_.setVectorUniform( "pplTexture", specular.c_str(),
                                         lightList_[i]->specular() );
    }
    // ========================================================================

    shaderStorage_.add( "flatshadeTexture" );
    shaderStorage_.use( "flatshadeTexture" );
    const Colorf lightColorShadedTex = { 1.0f, 1.0f, 1.0f };
    shaderStorage_.setVectorUniform(
        "flatshadeTexture", "lightcolor",
        algebra::vector3f{ lightColorShadedTex.r(), lightColorShadedTex.g(),
                           lightColorShadedTex.b() } );

    shaderStorage_.setVectorUniform( "flatshadeTexture", "lightpos",
                                     algebra::vector3f{ 0.0f, 10.0f, 0.0f } );
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
    shaderStorage_.use( "pplTexture" );
    shaderStorage_.setMatrixUniform( "pplTexture", "viewMatrix", GL_FALSE,
                                     camera().matrix<tire::OpenGLTag>() );

    shaderStorage_.setVectorUniform( "pplTexture", "eyePosition",
                                     camera().position() );

    for ( size_t i = 0; auto &buffer : buffersList_ ) {
        const Colorf bodyColor = bodyList_[i]->albedoColor();
        shaderStorage_.setVectorUniform(
            "pplTexture", "diffuseColor",
            algebra::vector3f{ bodyColor.r(), bodyColor.g(), bodyColor.b() } );

        texturesList_[i].bind();
        buffer.draw();
        buffer.release();
        ++i;
    }
}

}  // namespace tire::gl
