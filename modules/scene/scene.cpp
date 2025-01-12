
#include <fstream>
#include <array>
#include "geometry/polytope.h"

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_SCENE_CPP{ true };
#include "scene.h"
#include "config/config.h"

namespace tire {

Scene::Scene( const std::filesystem::path &fname, uv_loop_t *loop ) {
    const auto path = std::filesystem::path{ fname };

    io_ = std::make_shared<io::Factory>( loop );
    // remove
    io_->testFileOpen( path.string() );
    io_->testTimerEvent( 2000 );
    // ======
    log::info( "loading scene from file {}", fname.filename().string() );

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
            std::format( "file not found: {}\n", path.filename().string() ) );
    }

    // Parse json. Collect objects, cameras, lights and other scene entities
    process();
}

void Scene::process() {
    const auto basePath = Config::instance()->getBasePath();
    if ( scene_.contains( "objects" ) ) {
        const auto objects = scene_["objects"];
        for ( const auto &item : objects ) {
            const auto &type = item["type"];
            const auto &colorName = item["color"];
            const std::array<float, 3> pivotPosition = item["pivot_position"];
            const std::array<float, 3> pivotRotation = item["pivot_rotation"];
            const std::array<float, 3> pivotScale = item["pivot_scale"];
            const bool useMomentum = item["use_momentum"];
            const std::array<float, 3> momentum = item["momentum"];
            const auto textureFile = item["texture_diffuse"];
            if ( type == "box" ) {
                auto shapePtr = std::make_shared<BoxData>();
                auto node = std::make_shared<Node>( shapePtr );

                node->setColor( colorName );
                node->setUseMomentum( useMomentum );
                node->setMomentum( { momentum[0], momentum[1], momentum[2] } );
                node->setPivotScale(
                    { pivotScale[0], pivotScale[1], pivotScale[2] } );
                node->setPivotRotation(
                    { pivotRotation[0], pivotRotation[1], pivotRotation[2] } );
                node->setPivotOffset(
                    { pivotPosition[0], pivotPosition[1], pivotPosition[2] } );
                node->applyPivotTransormations();

                node->setTextureImage(
                    { basePath / "assets" / "textures" / textureFile } );

                nodeList_.push_back( std::move( node ) );

                log::debug<DEBUG_OUTPUT_SCENE_CPP>( "box added to scene" );
            } else if ( type == "frame" ) {
                auto shapePtr = std::make_shared<FrameData>();
                auto node = std::make_shared<Node>( shapePtr );

                node->setColor( colorName );
                node->setUseMomentum( useMomentum );
                node->setMomentum( { momentum[0], momentum[1], momentum[2] } );
                node->setPivotScale(
                    { pivotScale[0], pivotScale[1], pivotScale[2] } );
                node->setPivotRotation(
                    { pivotRotation[0], pivotRotation[1], pivotRotation[2] } );
                node->setPivotOffset(
                    { pivotPosition[0], pivotPosition[1], pivotPosition[2] } );
                node->applyPivotTransormations();

                node->setTextureImage(
                    { basePath / "assets" / "textures" / textureFile } );

                nodeList_.push_back( std::move( node ) );

                log::debug<DEBUG_OUTPUT_SCENE_CPP>( "frame added to scene" );
            } else if ( type == "diamond" ) {
                auto shapePtr = std::make_shared<DiamondData>();
                auto node = std::make_shared<Node>( shapePtr );

                node->setColor( colorName );
                node->setUseMomentum( useMomentum );
                node->setMomentum( { momentum[0], momentum[1], momentum[2] } );
                node->setPivotScale(
                    { pivotScale[0], pivotScale[1], pivotScale[2] } );
                node->setPivotRotation(
                    { pivotRotation[0], pivotRotation[1], pivotRotation[2] } );
                node->setPivotOffset(
                    { pivotPosition[0], pivotPosition[1], pivotPosition[2] } );
                node->applyPivotTransormations();

                node->setTextureImage(
                    { basePath / "assets" / "textures" / textureFile } );

                nodeList_.push_back( std::move( node ) );

                log::debug<DEBUG_OUTPUT_SCENE_CPP>( "frame added to scene" );
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
                const auto &aspect = 16.0f / 9.0f;  //item["aspect"];
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

void Scene::traverse() {
    for ( auto &node : nodeList_ ) {
        node->applyPivotTransormations();
    }
}

}  // namespace tire
