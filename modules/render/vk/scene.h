
#pragma once

#include <filesystem>

#include "scene/scene.h"

namespace tire::vk {

struct Scene final : tire::Scene {
    Scene( const std::filesystem::path &fname, uv_loop_t *loop );
};

}  // namespace tire::vk
