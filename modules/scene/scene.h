
#pragma once

#include <filesystem>
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

    void setActiveCamera( size_t id ) { activeCamera_ = id; };
    [[nodiscard]] Perspective &camera() const {
        return *cameras_[activeCamera_].get();
    };

    Flycam &flycam() { return player_; };

    virtual void submit() = 0;
    virtual void draw() = 0;

    void traverse( float frameDuration );

    float frameDuration_{};

private:
    void process();

private:
    nlohmann::json scene_;

protected:
    // float frameDuration_{};

    std::vector<std::shared_ptr<Body>> bodyList_{};
    std::vector<std::shared_ptr<Perspective>> cameras_{};

    int activeCamera_{ 0 };

    Flycam player_{};
};

}  // namespace tire
