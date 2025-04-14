
#pragma once

#include <filesystem>
#include "log/log.h"
#include "nlohmann/json.hpp"
#include "uv.h"

#include "body.h"
#include "camera.h"
#include "flycam.h"

namespace tire {

struct Scene {
    Scene( const std::filesystem::path &fname );

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

    [[nodiscard]] Flycam &camera() const {
        return *cameras_[activeCamera_].get();
    };

    [[nodiscard]] const Colorf &backgroundColor() const {
        return backgrounColor_;
    };

    virtual void submit() = 0;
    virtual void draw() = 0;
    virtual void clean() = 0;

    void traverse( float frameDuration );

private:
    void process();

private:
    nlohmann::json scene_;

protected:
    Colorf backgrounColor_{ "black" };

    std::vector<std::shared_ptr<Body>> bodyList_{};
    std::vector<std::shared_ptr<Flycam>> cameras_{};

    // One camera exist anyway
    int activeCamera_{ 0 };
};

}  // namespace tire
