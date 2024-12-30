
#include <fstream>

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
            const auto &position_x = item["position_x"];
            const auto &position_y = item["position_y"];
            const auto &position_z = item["position_z"];
            const auto &rotation_x = item["rotation_x"];
            const auto &rotation_y = item["rotation_y"];
            const auto &rotation_z = item["rotation_z"];
            const auto &scale_x = item["scale_x"];
            const auto &scale_y = item["scale_y"];
            const auto &scale_z = item["scale_z"];
            if ( type == "box" ) {
                auto node = std::make_shared<Node>( Box{} );
                node->setOffset(
                    algebra::vector3d{ position_x, position_y, position_z } );
                algebra::matrix3d rtn;
                rtn.rotation( rotation_x, rotation_y, rotation_z );
                node->setRotate( rtn );
                algebra::matrix3d scl;
                scl.idtt();
                node->setScale( scl );

                list_.push_back( node );

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
                const auto &position_x = item["position_x"];
                const auto &position_y = item["position_y"];
                const auto &position_z = item["position_z"];
                const auto &look_at_x = item["look_at_x"];
                const auto &look_at_y = item["look_at_y"];
                const auto &look_at_z = item["look_at_z"];
                const auto &fov = item["fov"];
                const auto &aspect = item["aspect"];
                const auto &ncp = item["ncp"];
                const auto &fcp = item["fcp"];
                auto camera = std::make_shared<Perspective>(
                    Perspective{ fov, aspect, ncp, fcp } );

                cameras_.push_back( camera );

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
