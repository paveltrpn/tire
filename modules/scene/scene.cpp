
module;

#include <format>
#include <filesystem>
#include <fstream>
#include <coroutine>

#include <vulkan/vulkan.h>

#include "nlohmann/json.hpp"

export module scene:scene;

import config;
import log;
import image;
import generator;
import :obj;
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

        log::info()( "loading scene from file {}", fname.string() );

        std::ifstream file{ path };
        if ( file ) {
            try {
                scene_ = nlohmann::json::parse( file );

                // Load meshes data.
                fillBaseMeshPool();

                // Parse json. Collect objects, cameras, lights and other scene entities
                process();
            } catch ( const nlohmann::json::parse_error &e ) {
                log::error(
                  "config json parse error\n"
                  "message:\t{}\n"
                  "exception id:\t{}\n"
                  "byte position of error:\t{}\n",
                  e.what(), e.id, e.byte );

                std::terminate();

            } catch ( const std::exception &e ) {
                log::fatal()( "exception: {}", e.what() );
            }
        } else {
            log::fatal()( "file not found: {}\n", path.string() );
        }
    }

    Scene( const Scene &other ) = delete;
    Scene( Scene &&other ) = delete;
    auto operator=( const Scene &other ) -> Scene & = delete;
    auto operator=( Scene &&other ) -> Scene & = delete;

    virtual ~Scene() = default;

    // Set active camera, first camera - 0.
    // One default camera exist anyway.
    auto setActiveCamera( size_t id ) -> void {
        if ( id < cameras_.size() ) {
            activeCamera_ = id;
        } else {
            activeCamera_ = 0;
            log::warning()( "try to get not existing camera - {}", id );
        }
    };

    // Switch to next camera.
    auto nextCamera() -> void {
        //
        setActiveCamera( ++activeCamera_ );
    }

    [[nodiscard]] auto camera() const -> Flycam & {
        //
        return *cameras_[activeCamera_].get();
    };

    [[nodiscard]] auto backgroundColor() const -> const Colorf & {
        //
        return backgrounColor_;
    };

    virtual void clean() = 0;

    auto traverse( float duration ) -> void {
        for ( size_t i{ 0 }; i < bodyList_.size(); ++i ) {
            bodyList_[i]->applyTransformations( duration );
        }

        // for ( auto &node : bodyList_ ) {
        // node->applyTransormations( duration );
        // }
    }

private:
    auto fillBaseMeshPool() -> void {
        const auto basePath = Config::instance()->getBasePath().string();
        const std::filesystem::path meshFilesPath = std::format( "{}/assets/base_mesh/", basePath );

        // Iterate over directory
        for ( auto &&entry : std::filesystem::directory_iterator{ meshFilesPath } ) {
            std::vector<std::string> retItem;
            // Take only "name" part of filename, i.e. except
            // extension and path.
            const auto &meshType = entry.path().stem().string();

            try {
                auto mesh = readWavefrontObjFile( entry.path().string() );
                baseMeshPool_.insert( { meshType, std::move( mesh ) } );

                log::info()( "mesh type \"{}\" added", meshType );

            } catch ( const std::exception &e ) {
                log::error( "Error reading mesh file: {}", e.what() );
                std::terminate();
            }
        }
    }

    [[nodiscard]]
    auto parseObjects( nlohmann::json objectsJson ) -> tire::generator<std::shared_ptr<tire::Body>> {
        for ( auto &&item : objectsJson ) {
            const auto &type = item[PARAM_OBJECT_TYPE];
            auto bodyMesh = baseMeshPool_[type];

            auto node = std::make_shared<Body>( std::move( bodyMesh ) );

            const std::array<float, 3> position = item[PARAM_OBJECT_POSITION];
            const std::array<float, 3> orientaion = item[PARAM_OBJECT_ORIENTATION];
            const std::array<float, 3> scale = item[PARAM_OBJECT_SCALE];
            const std::array<float, 3> velosity = item[PARAM_OBJECT_VELOCITY];
            const std::array<float, 3> torque = item[PARAM_OBJECT_TORQUE];
            const auto &albedoColor = item[PARAM_OBJECT_ALBEDO_COLOR];
            const std::string &materialName = item[PARAM_OBJECT_MATERIAL_NAME];

            node->setPosition( position )
              .setOrientation( orientaion )
              .setScale( scale )
              .setVelocity( velosity )
              .setTorque( torque )
              .setMaterialName( materialName )
              .setAlbedoColor( albedoColor );

            co_yield std::move( node );
        }
    }

    [[nodiscard]]
    auto parseCameras( nlohmann::json camerasJson ) -> tire::generator<std::shared_ptr<tire::Flycam>> {
        for ( auto &&item : camerasJson ) {
            const auto &type = item[PARAM_CAMERA_TYPE];
            if ( type == PARAM_CAMERA_PERSPECTIVE ) {
                auto camera = std::make_shared<Flycam>();

                const float azimuth = item[PARAM_CAMERA_AZIMUTH];
                const float elevation = item[PARAM_CAMERA_ELEVATION];

                // Unused
                const std::array<double, 3> target = item[PARAM_CAMERA_TARGET];

                const std::array<double, 3> eye = item[PARAM_CAMERA_EYE];
                const auto &fov = item[PARAM_CAMERA_FOV];
                const auto &aspect = item[PARAM_CAMERA_ASPECT];
                const auto &ncp = item[PARAM_CAMERA_NCP];
                const auto &fcp = item[PARAM_CAMERA_FCP];
                const std::string &name = item[PARAM_CAMERA_NAME];

                camera->setEye( algebra::vector3d{ eye } )
                  .setAzimuth( azimuth )
                  .setElevation( elevation )
                  .setFov( fov )
                  .setAspect( aspect )
                  .setNcp( ncp )
                  .setFcp( fcp )
                  .setName( name );

                co_yield std::move( camera );
            }
        }
    }

    [[nodiscard]]
    auto parseLights( nlohmann::json lightJson ) -> tire::generator<tire::OmniLight> {
        for ( auto &&item : lightJson ) {
            const auto &type = item[PARAM_LIGHT_TYPE];
            if ( type == PARAM_LIGHT_OMNI ) {
                auto light = OmniLight{};

                const std::array<float, 3> position = item[PARAM_OMNILIGHT_POSITION];
                const float constant = item[PARAM_OMNILIGHT_CONSTANT];
                const float linear = item[PARAM_OMNILIGHT_LINEAR];
                const float quadratic = item[PARAM_OMNILIGHT_QUADRATIC];
                const std::array<float, 3> ambient = item[PARAM_OMNILIGHT_AMBIENT];
                const std::array<float, 3> diffuse = item[PARAM_OMNILIGHT_DIFFUSE];
                const std::array<float, 3> specular = item[PARAM_OMNILIGHT_SPECULAR];

                light.setPosition( { position } )
                  .setConstant( constant )
                  .setLinear( linear )
                  .setQuadratic( quadratic )
                  .setAmbient( { ambient } )
                  .setDiffuse( { diffuse } )
                  .setSpecular( { specular } );

                co_yield std::move( light );
            }
        }
    }

    auto process() -> void {
        // Read "environment section"
        const auto &environment = scene_[PARAM_ENVIRONMENT];
        const auto &bg = environment[PARAM_BACKGROUND_COLOR];
        backgrounColor_ = Colorf( bg );

        // Read "objects" section
        if ( scene_.contains( PARAM_OBJECTS ) ) {
            auto objectsGenerator = parseObjects( scene_[PARAM_OBJECTS] );
            for ( auto &&body : objectsGenerator ) {
                bodyList_.push_back( std::move( body ) );
            }
        } else {
            throw std::runtime_error( "there is can't be scene without objects!" );
        }

        // Read "cameras" section
        if ( scene_.contains( PARAM_CAMERAS ) ) {
            auto cameraGenerator = parseCameras( scene_[PARAM_CAMERAS] );
            for ( auto &&camera : cameraGenerator ) {
                cameras_.push_back( std::move( camera ) );
            }
        } else {
            throw std::runtime_error( "there is can't be scene without cameras!" );
        }

        // Read "lights" section
        if ( scene_.contains( PARAM_LIGHTS ) ) {
            auto lightsGenerator = parseLights( scene_[PARAM_LIGHTS] );
            for ( auto &&light : lightsGenerator ) {
                lightList_.push_back( std::move( light ) );
            }
        } else {
            throw std::runtime_error( "there is can't be scene without lights!" );
        }
    }

private:
    nlohmann::json scene_;

protected:
    Colorf backgrounColor_{ "black" };

    std::unordered_map<std::string, std::shared_ptr<SeparatedBuffersMesh>> baseMeshPool_{};

    std::vector<std::shared_ptr<Body>> bodyList_{};
    std::vector<std::shared_ptr<Flycam>> cameras_{};
    std::vector<OmniLight> lightList_{};

    // One camera exist anyway
    int activeCamera_{ 0 };
};

}  // namespace tire
