
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

    [[nodiscard]] Camera &getCamera( size_t id ) const {
        return *( camera_[id] ).get();
    };

private:
    void process();

private:
    nlohmann::json scene_;
    std::vector<std::shared_ptr<Node>> list_;
    std::vector<std::shared_ptr<Camera>> camera_{};
};

}  // namespace tire
