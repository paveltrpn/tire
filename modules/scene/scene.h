
#pragma once

#include <list>
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

    ~Scene();

    [[nodiscard]] Camera &getCamera() const { return *camera_.get(); };

private:
    void process();
    void appendToRenderList( std::shared_ptr<tire::Node> node );

private:
    nlohmann::json scene_;
    std::list<std::shared_ptr<Node>> list_;
    std::shared_ptr<Camera> camera_{ nullptr };
};

}  // namespace tire
