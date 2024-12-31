
#include <fstream>
#include <array>
#include "geometry/polytope.h"

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_SCENE_CPP{ true };
#include "scene.h"

namespace tire {

Scene::Scene( const std::filesystem::path &fname ) {
    const auto path = std::filesystem::path{ fname };
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
        throw std::runtime_error(
            std::format( "file not found: {}\n", path.string() ) );
    }
    process();
}

void Scene::process() {
    if ( scene_.contains( "objects" ) ) {
        const auto objects = scene_["objects"];
        for ( const auto &item : objects ) {
            const auto &type = item["type"];
            const std::array<float, 3> defPosition = item["def_position"];
            const std::array<float, 3> defRotation = item["def_rotation"];
            const std::array<float, 3> defScale = item["def_scale"];
            if ( type == "box" ) {
                auto node = std::make_shared<Node>( Box{} );

                algebra::matrix3f rtn;
                rtn.rotation( defRotation[0], defRotation[1], defRotation[2] );
                node->setRotate( rtn );
                node->applyRotate();

                algebra::matrix3f scl;
                scl.idtt();
                node->setScale( scl );

                node->setOffset( algebra::vector3f{
                    defPosition[0], defPosition[1], defPosition[2] } );
                node->applyMove();

                nodeList_.push_back( std::move( node ) );

                log::debug<DEBUG_OUTPUT_SCENE_CPP>( "box added to scene" );
            }
        }
    } else {
        throw std::runtime_error( "there is can't be scene without objects!" );
    }

    if ( scene_.contains( "cameras" ) ) {
        const auto cameras = scene_["cameras"];
        for ( const auto &item : cameras ) {
            const auto &type = item["type"];
            if ( type == "perspective" ) {
                const std::array<float, 3> position = item["position"];
                const std::array<float, 3> lookAt = item["look_at"];
                const auto &fov = item["fov"];
                const auto &aspect = item["aspect"];
                const auto &ncp = item["ncp"];
                const auto &fcp = item["fcp"];
                auto camera = std::make_shared<Perspective>(
                    Perspective{ fov, aspect, ncp, fcp } );
                camera->move( { position[0], position[1], position[2] } );
                cameras_.push_back( std::move( camera ) );

                log::debug<DEBUG_OUTPUT_SCENE_CPP>(
                    "perspective camera added to scene" );
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

}  // namespace tire
