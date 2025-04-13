
#include <fstream>
#include <array>
#include "algebra/vector3.h"
#include "geometry/polytope.h"

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_SCENE_CPP{ true };
#include "scene.h"
#include "constants.h"

import config;

namespace tire {

Scene::Scene( const std::filesystem::path &fname ) {
    const auto path = std::filesystem::path{ fname };

    log::info( "Scene === loading scene from file {}",
               fname.filename().string() );

    std::ifstream file{ path };
    if ( file ) {
        try {
            scene_ = nlohmann::json::parse( file );
        } catch ( const nlohmann::json::parse_error &e ) {
            log::error(
                "config json parse error\n"
                "message:\t{}\n"
                "exception id:\t{}\n"
                "byte position of error:\t{}\n",
                e.what(), e.id, e.byte );
        }
    } else {
        throw std::runtime_error( std::format(
            "Scene ===  file not found: {}\n", path.filename().string() ) );
    }

    // Parse json. Collect objects, cameras, lights and other scene entities
    process();
}

void Scene::process() {
    const auto basePath = Config::instance()->getBasePath();
    if ( scene_.contains( constants::scene::PARAM_OBJECTS ) ) {
        const auto objects = scene_[constants::scene::PARAM_OBJECTS];
        for ( const auto &item : objects ) {
            const auto &type = item[constants::scene::PARAM_OBJECT_TYPE];

            // Body spatial information
            const std::array<float, 3> position =
                item[constants::scene::PARAM_OBJECT_POSITION];
            const std::array<float, 3> orientaion =
                item[constants::scene::PARAM_OBJECT_ORIENTATION];
            const std::array<float, 3> scale =
                item[constants::scene::PARAM_OBJECT_SCALE];
            const std::array<float, 3> velosity =
                item[constants::scene::PARAM_OBJECT_VELOCITY];
            const std::array<float, 3> torque =
                item[constants::scene::PARAM_OBJECT_TORQUE];

            // Body material information
            const auto &albedoColor =
                item[constants::scene::PARAM_OBJECT_ALBEDO_COLOR];
            const auto &albedoTextureFile =
                item[constants::scene::PARAM_OBJECT_ALBEDO_TEXTURE];

            // Body vertecies data
            auto node = std::make_shared<Body>();
            if ( type == constants::scene::PARAM_OBJECT_TYPE_BOX ) {
                auto shapePtr = std::make_shared<BoxData>();
                node->setShapeData( std::move( shapePtr ) );
                log::debug<DEBUG_OUTPUT_SCENE_CPP>(
                    "Scene === \"{}\" added to scene",
                    constants::scene::PARAM_OBJECT_TYPE_BOX );
            } else if ( type == constants::scene::PARAM_OBJECT_TYPE_FRAME ) {
                auto shapePtr = std::make_shared<FrameData>();
                node->setShapeData( std::move( shapePtr ) );
                log::debug<DEBUG_OUTPUT_SCENE_CPP>(
                    "Scene ===  \"{}\" added to scene",
                    constants::scene::PARAM_OBJECT_TYPE_FRAME );
            } else if ( type == constants::scene::PARAM_OBJECT_TYPE_DIAMOND ) {
                auto shapePtr = std::make_shared<DiamondData>();
                node->setShapeData( std::move( shapePtr ) );
                log::debug<DEBUG_OUTPUT_SCENE_CPP>(
                    "Scene ===  \"{}\" added to scene",
                    constants::scene::PARAM_OBJECT_TYPE_DIAMOND );
            }

            // Set body properties
            node->setPosition( position );
            node->setOrientation( orientaion );
            node->setScale( scale );
            node->setVelocity( velosity );
            node->setTorque( torque );

            node->setAlbedoColor( albedoColor );
            node->setAlbedoTextureImage(
                { basePath / "assets" / "textures" / albedoTextureFile } );

            // Append body to list
            bodyList_.push_back( std::move( node ) );
        }
    } else {
        throw std::runtime_error( "there is can't be scene without objects!" );
    }

    if ( scene_.contains( constants::scene::PARAM_CAMERAS ) ) {
        const auto cameras = scene_[constants::scene::PARAM_CAMERAS];
        for ( const auto &item : cameras ) {
            const auto &type = item[constants::scene::PARAM_CAMERA_TYPE];
            if ( type == constants::scene::PARAM_CAMERA_PERSPECTIVE ) {
                const std::array<float, 3> eye =
                    item[constants::scene::PARAM_CAMERA_EYE];
                const auto eyev = algebra::vector3f{ eye };

                const std::array<float, 3> target =
                    item[constants::scene::PARAM_CAMERA_TARGET];
                const auto targetv = algebra::vector3f{ target };

                const float azimuth =
                    item[constants::scene::PARAM_CAMERA_AZIMUTH];

                const float elevation =
                    item[constants::scene::PARAM_CAMERA_ELEVATION];

                const auto &fov = item[constants::scene::PARAM_CAMERA_FOV];
                const auto &aspect =
                    16.0f /
                    9.0f;  //item[constants::scene::PARAM_CAMERA_ASPECT];
                const auto &ncp = item[constants::scene::PARAM_CAMERA_NCP];
                const auto &fcp = item[constants::scene::PARAM_CAMERA_FCP];

                auto camera =
                    std::make_shared<Flycam>( eye, azimuth, elevation );

                camera->setFov( fov );
                camera->setAspect( aspect );
                camera->setNcp( ncp );
                camera->setFcp( fcp );

                cameras_.push_back( std::move( camera ) );

                log::debug<DEBUG_OUTPUT_SCENE_CPP>(
                    "Scene === perspective camera added to scene" );
            }
        }
    } else {
        throw std::runtime_error( "there is can't be scene without cameras!" );
    }

    // if ( scene_.contains( "lights" ) ) {
    // } else {
    //     throw std::runtime_error( "there is can't be scene without lights!" );
    // }
}

void Scene::traverse( float duration ) {
    // Update global time flow
    frameDuration_ = duration;

    for ( auto &node : bodyList_ ) {
        node->applyTransormations();
    }
}

}  // namespace tire
