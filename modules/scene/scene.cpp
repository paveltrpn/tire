
#include <fstream>
#include <array>
#include <format>

#include "algebra/vector3.h"
#include "geometry/polytope.h"

#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_SCENE_CPP{ true };
#include "scene.h"
#include "constants.h"

import config;

namespace tire {

Scene::Scene( const std::filesystem::path& fname ) {
    const auto path = std::filesystem::path{ fname };

    log::info( "Scene === loading scene from file {}", fname.string() );

    std::ifstream file{ path };
    if ( file ) {
        try {
            scene_ = nlohmann::json::parse( file );
        } catch ( const nlohmann::json::parse_error& e ) {
            log::error(
                "config json parse error\n"
                "message:\t{}\n"
                "exception id:\t{}\n"
                "byte position of error:\t{}\n",
                e.what(), e.id, e.byte );
        }
    } else {
        throw std::runtime_error(
            std::format( "Scene ===  file not found: {}\n", path.string() ) );
    }

    // Load meshes data.
    fillMeshBank();

    // Parse json. Collect objects, cameras, lights and other scene entities
    process();
}

void Scene::fillMeshBank() {
    const auto basePath = Config::instance()->getBasePath().string();
    const std::filesystem::path meshFlesPath =
        std::format( "{}/assets/mesh/", basePath );

    // Iterate over directory
    for ( auto&& entry : std::filesystem::directory_iterator{ meshFlesPath } ) {
        std::vector<std::string> retItem;
        // Take only "name" part of filename, i.e. except
        // extension and path.
        const auto& fileName = entry.path().stem().string();

        auto mesh = std::make_shared<Mesh>( entry.path().string() );
        meshBank_.insert( { fileName, std::move( mesh ) } );
    }
}

void Scene::process() {
    // Read "environment section"
    const auto& environment = scene_[constants::scene::PARAM_ENVIRONMENT];
    const auto& bg = environment[constants::scene::PARAM_BACKGROUND_COLOR];
    backgrounColor_ = Colorf( bg );

    // Read "objects" section
    if ( scene_.contains( constants::scene::PARAM_OBJECTS ) ) {
        const auto objects = scene_[constants::scene::PARAM_OBJECTS];
        for ( const auto& item : objects ) {
            const auto& type = item[constants::scene::PARAM_OBJECT_TYPE];

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
            const auto& albedoColor =
                item[constants::scene::PARAM_OBJECT_ALBEDO_COLOR];

            const std::string& materialName =
                item[constants::scene::PARAM_OBJECT_MATERIAL_NAME];

            // Body vertices data
            auto node = std::make_shared<Body>();
            if ( type == constants::scene::PARAM_OBJECT_TYPE_BOX ) {
                auto shapePtr = std::make_shared<BoxData>();
                node->setShapeData( std::move( shapePtr ) );

            } else if ( type == constants::scene::PARAM_OBJECT_TYPE_FRAME ) {
                auto shapePtr = std::make_shared<FrameData>();
                node->setShapeData( std::move( shapePtr ) );

            } else if ( type == constants::scene::PARAM_OBJECT_TYPE_DIAMOND ) {
                auto shapePtr = std::make_shared<DiamondData>();
                node->setShapeData( std::move( shapePtr ) );

            } else if ( type == constants::scene::PARAM_OBJECT_TYPE_WALL01 ) {
                auto shapePtr = std::make_shared<Wall01Data>();
                node->setShapeData( std::move( shapePtr ) );

            } else if ( type == constants::scene::PARAM_OBJECT_TYPE_ARCH01 ) {
                auto shapePtr = std::make_shared<Arch01Data>();
                node->setShapeData( std::move( shapePtr ) );

            } else if ( type ==
                        constants::scene::PARAM_OBJECT_TYPE_PRISMHEXA ) {
                auto shapePtr = std::make_shared<PrismhexaData>();
                node->setShapeData( std::move( shapePtr ) );

            } else if ( type == constants::scene::PARAM_OBJECT_TYPE_PRISM ) {
                auto shapePtr = std::make_shared<PrismData>();
                node->setShapeData( std::move( shapePtr ) );

            } else if ( type ==
                        constants::scene::PARAM_OBJECT_TYPE_PYRAMIDCUT ) {
                auto shapePtr = std::make_shared<PyramidcutData>();
                node->setShapeData( std::move( shapePtr ) );
            }

            // Set body properties
            node->setPosition( position );
            node->setOrientation( orientaion );
            node->setScale( scale );
            node->setVelocity( velosity );
            node->setTorque( torque );
            node->setMaterialName( materialName );
            node->setAlbedoColor( albedoColor );

            // Append body to list
            bodyList_.push_back( std::move( node ) );
        }
    } else {
        throw std::runtime_error( "there is can't be scene without objects!" );
    }

    // Read "cameras" section
    if ( scene_.contains( constants::scene::PARAM_CAMERAS ) ) {
        const auto cameras = scene_[constants::scene::PARAM_CAMERAS];
        for ( const auto& item : cameras ) {
            const auto& type = item[constants::scene::PARAM_CAMERA_TYPE];
            if ( type == constants::scene::PARAM_CAMERA_PERSPECTIVE ) {
                const std::array<double, 3> eye =
                    item[constants::scene::PARAM_CAMERA_EYE];
                const auto eyev = algebra::vector3d{ eye };

                // Unused
                const std::array<double, 3> target =
                    item[constants::scene::PARAM_CAMERA_TARGET];

                const float azimuth =
                    item[constants::scene::PARAM_CAMERA_AZIMUTH];

                const float elevation =
                    item[constants::scene::PARAM_CAMERA_ELEVATION];

                const auto& fov = item[constants::scene::PARAM_CAMERA_FOV];
                const auto& aspect =
                    item[constants::scene::PARAM_CAMERA_ASPECT];
                const auto& ncp = item[constants::scene::PARAM_CAMERA_NCP];
                const auto& fcp = item[constants::scene::PARAM_CAMERA_FCP];

                const std::string& name =
                    item[constants::scene::PARAM_CAMERA_NAME];

                auto camera =
                    std::make_shared<Flycam>( eye, azimuth, elevation );

                camera->setFov( fov )
                    .setAspect( aspect )
                    .setNcp( ncp )
                    .setFcp( fcp )
                    .setName( name );

                cameras_.push_back( std::move( camera ) );
            }
        }
    } else {
        throw std::runtime_error( "there is can't be scene without cameras!" );
    }

    // Read "lights" section
    if ( scene_.contains( constants::scene::PARAM_LIGHTS ) ) {
        const auto lights = scene_[constants::scene::PARAM_LIGHTS];
        for ( const auto& item : lights ) {
            const auto& type = item[constants::scene::PARAM_LIGHT_TYPE];
            if ( type == constants::scene::PARAM_LIGHT_OMNI ) {
                const std::array<float, 3> position =
                    item[constants::scene::PARAM_OMNILIGHT_POSITION];

                const float constant =
                    item[constants::scene::PARAM_OMNILIGHT_CONSTANT];

                const float linear =
                    item[constants::scene::PARAM_OMNILIGHT_LINEAR];

                const float quadratic =
                    item[constants::scene::PARAM_OMNILIGHT_QUADRATIC];

                const std::array<float, 3> ambient =
                    item[constants::scene::PARAM_OMNILIGHT_AMBIENT];

                const std::array<float, 3> diffuse =
                    item[constants::scene::PARAM_OMNILIGHT_DIFFUSE];

                const std::array<float, 3> specular =
                    item[constants::scene::PARAM_OMNILIGHT_SPECULAR];

                auto light = std::make_shared<OmniLight<float>>();

                light->setPosition( { position } )
                    .setConstant( constant )
                    .setLinear( linear )
                    .setQuadratic( quadratic )
                    .setAmbient( { ambient } )
                    .setDiffuse( { diffuse } )
                    .setSpecular( { specular } );

                lightList_.push_back( light );
            }
        }
    } else {
        throw std::runtime_error( "there is can't be scene without lights!" );
    }
}

void Scene::traverse( float duration ) {
    for ( size_t i{ 0 }; i < bodyList_.size(); ++i ) {
        bodyList_[i]->applyTransormations( duration );
    }

    // for ( auto &node : bodyList_ ) {
    // node->applyTransormations( duration );
    // }
}

}  // namespace tire
