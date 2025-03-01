
#pragma once

#include <filesystem>
#include "nlohmann/json.hpp"
#include "uv.h"

#include "body.h"
#include "camera.h"

namespace tire {

struct Scene {
    Scene( const std::filesystem::path &fname );

    Scene( const Scene &other ) = delete;
    Scene( Scene &&other ) = delete;
    Scene &operator=( const Scene &other ) = delete;
    Scene &operator=( Scene &&other ) = delete;

    virtual ~Scene() = default;

    void setActiveCamera( size_t id ) { activeCamera_ = id; };
    [[nodiscard]] std::shared_ptr<Camera> camera() const {
        return cameras_[activeCamera_];
    };

    void traverse( float frameDuration );

private:
    void process();

private:
    nlohmann::json scene_;

protected:
    float frameDuration_{};

    std::vector<std::shared_ptr<Body>> bodyList_{};
    std::vector<std::shared_ptr<Camera>> cameras_{};

    int activeCamera_{ 0 };
};

}  // namespace tire
