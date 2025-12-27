
module;

#include <filesystem>
#include <fstream>

#include <vulkan/vulkan.h>

#include "nlohmann/json.hpp"
#include "config/config.h"
#include "geometry/polytope.h"

export module scene:scene;

import image;
import log;

import :mesh;
import :body;
import :light;
import :flycam;

namespace tire {

constexpr char PARAM_ENVIRONMENT[] = "environment";
constexpr char PARAM_OBJECTS[] = "objects";
constexpr char PARAM_CAMERAS[] = "cameras";
constexpr char PARAM_LIGHTS[] = "lights";

constexpr char PARAM_BACKGROUND_COLOR[] = "background_color";

constexpr char PARAM_OBJECT_TYPE_BOX[] = "box";
constexpr char PARAM_OBJECT_TYPE_FRAME[] = "frame";
constexpr char PARAM_OBJECT_TYPE_DIAMOND[] = "diamond";
constexpr char PARAM_OBJECT_TYPE_WALL01[] = "wall01";
constexpr char PARAM_OBJECT_TYPE_ARCH01[] = "arch01";
constexpr char PARAM_OBJECT_TYPE_PRISM[] = "prism";
constexpr char PARAM_OBJECT_TYPE_PRISMHEXA[] = "prismhexa";
constexpr char PARAM_OBJECT_TYPE_PYRAMIDCUT[] = "pyramidcut";

constexpr char PARAM_OBJECT_TYPE[] = "type";

constexpr char PARAM_OBJECT_POSITION[] = "position";
constexpr char PARAM_OBJECT_ORIENTATION[] = "orientation";
constexpr char PARAM_OBJECT_SCALE[] = "scale";
constexpr char PARAM_OBJECT_VELOCITY[] = "velocity";
constexpr char PARAM_OBJECT_TORQUE[] = "torque";

constexpr char PARAM_OBJECT_ALBEDO_COLOR[] = "albedo_color";
constexpr char PARAM_OBJECT_SPECULAR_COLOR[] = "specular_color";
constexpr char PARAM_OBJECT_ALBEDO_TEXTURE[] = "albedo_texture";
constexpr char PARAM_OBJECT_MATERIAL_NAME[] = "material_name";

constexpr char PARAM_CAMERA_TYPE[] = "type";
constexpr char PARAM_CAMERA_PERSPECTIVE[] = "perspective";
constexpr char PARAM_CAMERA_EYE[] = "eye";
constexpr char PARAM_CAMERA_TARGET[] = "target";
constexpr char PARAM_CAMERA_AZIMUTH[] = "azimuth";
constexpr char PARAM_CAMERA_ELEVATION[] = "elevation";
constexpr char PARAM_CAMERA_FOV[] = "fov";
constexpr char PARAM_CAMERA_ASPECT[] = "aspect";
constexpr char PARAM_CAMERA_NCP[] = "ncp";
constexpr char PARAM_CAMERA_FCP[] = "fcp";
constexpr char PARAM_CAMERA_NAME[] = "name";

constexpr char PARAM_LIGHT_TYPE[] = "type";
constexpr char PARAM_LIGHT_OMNI[] = "omni";
constexpr char PARAM_OMNILIGHT_POSITION[] = "position";
constexpr char PARAM_OMNILIGHT_CONSTANT[] = "constant";
constexpr char PARAM_OMNILIGHT_LINEAR[] = "linear";
constexpr char PARAM_OMNILIGHT_QUADRATIC[] = "quadratic";
constexpr char PARAM_OMNILIGHT_AMBIENT[] = "ambient";
constexpr char PARAM_OMNILIGHT_DIFFUSE[] = "diffuse";
constexpr char PARAM_OMNILIGHT_SPECULAR[] = "specular";

constexpr char PARAM_NAME_GRAVITY_VEC[] = "gravity_vec";

export struct Scene {
    Scene( const std::filesystem::path &fname ) {
        const auto path = std::filesystem::path{ fname };

        log::info( "Scene === loading scene from file {}", fname.string() );

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
            throw std::runtime_error( std::format( "Scene ===  file not found: {}\n", path.string() ) );
        }

        // Load meshes data.
        fillMeshBank();

        // Parse json. Collect objects, cameras, lights and other scene entities
        process();
    }

    Scene( const Scene &other ) = delete;
    Scene( Scene &&other ) = delete;
    Scene &operator=( const Scene &other ) = delete;
    Scene &operator=( Scene &&other ) = delete;

    virtual ~Scene() = default;

    // Set active camera, first camera - 0.
    // One default camera exist anyway.
    void setActiveCamera( size_t id ) {
        if ( id < cameras_.size() ) {
            activeCamera_ = id;
        } else {
            activeCamera_ = 0;
            log::warning( "Scene === try to get not existing camera - {}", id );
        }
    };

    // Switch to next camera.
    void nextCamera() { setActiveCamera( ++activeCamera_ ); }

    [[nodiscard]] Flycam &camera() const { return *cameras_[activeCamera_].get(); };

    [[nodiscard]] const Colorf &backgroundColor() const { return backgrounColor_; };

    virtual void submit() = 0;
    virtual void draw() = 0;
    virtual void clean() = 0;

    void traverse( float duration ) {
        for ( size_t i{ 0 }; i < bodyList_.size(); ++i ) {
            bodyList_[i]->applyTransormations( duration );
        }

        // for ( auto &node : bodyList_ ) {
        // node->applyTransormations( duration );
        // }
    }

private:
    void fillMeshBank() {
        const auto basePath = Config::instance()->getBasePath().string();
        const std::filesystem::path meshFlesPath = std::format( "{}/assets/mesh/", basePath );

        // Iterate over directory
        for ( auto &&entry : std::filesystem::directory_iterator{ meshFlesPath } ) {
            std::vector<std::string> retItem;
            // Take only "name" part of filename, i.e. except
            // extension and path.
            const auto &fileName = entry.path().stem().string();

            auto mesh = std::make_shared<Mesh>( entry.path().string() );
            meshBank_.insert( { fileName, std::move( mesh ) } );
        }
    }

    void process() {
        // Read "environment section"
        const auto &environment = scene_[PARAM_ENVIRONMENT];
        const auto &bg = environment[PARAM_BACKGROUND_COLOR];
        backgrounColor_ = Colorf( bg );

        // Read "objects" section
        if ( scene_.contains( PARAM_OBJECTS ) ) {
            const auto objects = scene_[PARAM_OBJECTS];
            for ( const auto &item : objects ) {
                const auto &type = item[PARAM_OBJECT_TYPE];

                // Body spatial information
                const std::array<float, 3> position = item[PARAM_OBJECT_POSITION];

                const std::array<float, 3> orientaion = item[PARAM_OBJECT_ORIENTATION];

                const std::array<float, 3> scale = item[PARAM_OBJECT_SCALE];

                const std::array<float, 3> velosity = item[PARAM_OBJECT_VELOCITY];

                const std::array<float, 3> torque = item[PARAM_OBJECT_TORQUE];

                // Body material information
                const auto &albedoColor = item[PARAM_OBJECT_ALBEDO_COLOR];

                const std::string &materialName = item[PARAM_OBJECT_MATERIAL_NAME];

                // Body vertices data
                auto node = std::make_shared<Body>();
                if ( type == PARAM_OBJECT_TYPE_BOX ) {
                    auto shapePtr = std::make_shared<BoxData>();
                    node->setShapeData( std::move( shapePtr ) );

                } else if ( type == PARAM_OBJECT_TYPE_FRAME ) {
                    auto shapePtr = std::make_shared<FrameData>();
                    node->setShapeData( std::move( shapePtr ) );

                } else if ( type == PARAM_OBJECT_TYPE_DIAMOND ) {
                    auto shapePtr = std::make_shared<DiamondData>();
                    node->setShapeData( std::move( shapePtr ) );

                } else if ( type == PARAM_OBJECT_TYPE_WALL01 ) {
                    auto shapePtr = std::make_shared<Wall01Data>();
                    node->setShapeData( std::move( shapePtr ) );

                } else if ( type == PARAM_OBJECT_TYPE_ARCH01 ) {
                    auto shapePtr = std::make_shared<Arch01Data>();
                    node->setShapeData( std::move( shapePtr ) );

                } else if ( type == PARAM_OBJECT_TYPE_PRISMHEXA ) {
                    auto shapePtr = std::make_shared<PrismhexaData>();
                    node->setShapeData( std::move( shapePtr ) );

                } else if ( type == PARAM_OBJECT_TYPE_PRISM ) {
                    auto shapePtr = std::make_shared<PrismData>();
                    node->setShapeData( std::move( shapePtr ) );

                } else if ( type == PARAM_OBJECT_TYPE_PYRAMIDCUT ) {
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
        if ( scene_.contains( PARAM_CAMERAS ) ) {
            const auto cameras = scene_[PARAM_CAMERAS];
            for ( const auto &item : cameras ) {
                const auto &type = item[PARAM_CAMERA_TYPE];
                if ( type == PARAM_CAMERA_PERSPECTIVE ) {
                    const std::array<double, 3> eye = item[PARAM_CAMERA_EYE];
                    const auto eyev = algebra::vector3d{ eye };

                    // Unused
                    const std::array<double, 3> target = item[PARAM_CAMERA_TARGET];

                    const float azimuth = item[PARAM_CAMERA_AZIMUTH];

                    const float elevation = item[PARAM_CAMERA_ELEVATION];

                    const auto &fov = item[PARAM_CAMERA_FOV];
                    const auto &aspect = item[PARAM_CAMERA_ASPECT];
                    const auto &ncp = item[PARAM_CAMERA_NCP];
                    const auto &fcp = item[PARAM_CAMERA_FCP];

                    const std::string &name = item[PARAM_CAMERA_NAME];

                    auto camera = std::make_shared<Flycam>( eye, azimuth, elevation );

                    camera->setFov( fov ).setAspect( aspect ).setNcp( ncp ).setFcp( fcp ).setName( name );

                    cameras_.push_back( std::move( camera ) );
                }
            }
        } else {
            throw std::runtime_error( "there is can't be scene without cameras!" );
        }

        // Read "lights" section
        if ( scene_.contains( PARAM_LIGHTS ) ) {
            const auto lights = scene_[PARAM_LIGHTS];
            for ( const auto &item : lights ) {
                const auto &type = item[PARAM_LIGHT_TYPE];
                if ( type == PARAM_LIGHT_OMNI ) {
                    const std::array<float, 3> position = item[PARAM_OMNILIGHT_POSITION];

                    const float constant = item[PARAM_OMNILIGHT_CONSTANT];

                    const float linear = item[PARAM_OMNILIGHT_LINEAR];

                    const float quadratic = item[PARAM_OMNILIGHT_QUADRATIC];

                    const std::array<float, 3> ambient = item[PARAM_OMNILIGHT_AMBIENT];

                    const std::array<float, 3> diffuse = item[PARAM_OMNILIGHT_DIFFUSE];

                    const std::array<float, 3> specular = item[PARAM_OMNILIGHT_SPECULAR];

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

private:
    nlohmann::json scene_;

protected:
    Colorf backgrounColor_{ "black" };

    std::vector<std::shared_ptr<Body>> bodyList_{};
    std::vector<std::shared_ptr<OmniLight<float>>> lightList_{};
    std::vector<std::shared_ptr<Flycam>> cameras_{};

    // One camera exist anyway
    int activeCamera_{ 0 };

    std::unordered_map<std::string, std::shared_ptr<Mesh>> meshBank_{};
};

}  // namespace tire
