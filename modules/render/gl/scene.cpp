
#include "scene.h"
#include <GL/gl.h>
#include <GL/glcorearb.h>
#include "functions.h"
#include "render/gl/material.h"
#include "render/gl/texture.h"

import config;

namespace tire::gl {

Scene::Scene( const std::filesystem::path &fname )
    : tire::Scene{ fname } {
    const auto nodeListSize = bodyList_.size();
    buffersList_.reserve( nodeListSize );

    for ( size_t i{}; i < nodeListSize; ++i ) {
        // Generate OpneGl vertex buffers.
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

        // Load textures that requres by body from bodys list.
        const auto &materialName = bodyList_[i]->materialName();
        if ( !materialSet_.contains( materialName ) ) {
            materialSet_[materialName] = TextureSet{ materialName };
        }
    }

    // Load shaders
    pplTexture.init( "pplTexture" );
    pplTexture.use();
    pplTexture.addUniform( std::vector<std::string>{
        "viewMatrix", "eyePosition", "diffuseColor", "lightsCount" } );
    for ( size_t i{ 0 }; i < lightList_.size(); ++i ) {
        pplTexture.addUniform( std::vector<std::string>{
            std::format( "omniLights[{}].position", i ),
            std::format( "omniLights[{}].position", i ),
            std::format( "omniLights[{}].constant", i ),
            std::format( "omniLights[{}].linear", i ),
            std::format( "omniLights[{}].quadratic", i ),
            std::format( "omniLights[{}].ambient", i ),
            std::format( "omniLights[{}].diffuse", i ),
            std::format( "omniLights[{}].specular", i ) } );
    }

    pplTexture.setScalarUniform( "lightsCount", int( lightList_.size() ) );
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
    pplTexture.use();
    for ( size_t i{ 0 }; i < lightList_.size(); ++i ) {
        pplTexture.setVectorUniform(
            std::format( "omniLights[{}].position", i ).c_str(),
            lightList_[i]->position() );

        pplTexture.setScalarUniform(
            std::format( "omniLights[{}].constant", i ).c_str(),
            lightList_[i]->constant() );

        pplTexture.setScalarUniform(
            std::format( "omniLights[{}].linear", i ).c_str(),
            lightList_[i]->linear() );

        pplTexture.setScalarUniform(
            std::format( "omniLights[{}].quadratic", i ).c_str(),
            lightList_[i]->quadratic() );

        pplTexture.setVectorUniform(
            std::format( "omniLights[{}].ambient", i ).c_str(),
            lightList_[i]->ambient() );

        pplTexture.setVectorUniform(
            std::format( "omniLights[{}].diffuse", i ).c_str(),
            lightList_[i]->diffuse() );

        pplTexture.setVectorUniform(
            std::format( "omniLights[{}].specular", i ).c_str(),
            lightList_[i]->specular() );
    }
    pplTexture.setMatrixUniform( "viewMatrix", GL_FALSE,
                                 camera().matrix<tire::OpenGLTag>() );
    pplTexture.setVectorUniform( "eyePosition", camera().position() );

    for ( size_t i = 0; auto &buffer : buffersList_ ) {
        const Colorf bodyColor = bodyList_[i]->albedoColor();
        pplTexture.setVectorUniform(
            "diffuseColor",
            algebra::vector3f{ bodyColor.r(), bodyColor.g(), bodyColor.b() } );

        materialSet_[bodyList_[i]->materialName()].bind();

        buffer.draw();
        buffer.release();

        ++i;
    }
}

}  // namespace tire::gl
