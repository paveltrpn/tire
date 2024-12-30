
#pragma once

#include <filesystem>
#include "nlohmann/json.hpp"

#include "node.h"
#include "camera/camera.h"

namespace tire {

struct Scene final {
    Scene( const std::filesystem::path &fname );

    Scene( const Scene &other ) = delete;
    Scene( Scene &&other ) = delete;
    Scene &operator=( const Scene &other ) = delete;
    Scene &operator=( Scene &&other ) = delete;

    ~Scene() = default;

    [[nodiscard]] std::shared_ptr<Camera> getCamera( size_t id ) const {
        return cameras_[id];
    };

private:
    void process();

private:
    nlohmann::json scene_;
    std::vector<std::shared_ptr<Node>> list_;
    std::vector<std::shared_ptr<Camera>> cameras_{};
};

}  // namespace tire
