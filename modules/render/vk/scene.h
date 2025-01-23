
#pragma once

#include <filesystem>

#include "scene/scene.h"

namespace tire::vk {

struct Scene final : tire::Scene {
    Scene( const std::filesystem::path &fname );
};

}  // namespace tire::vk
