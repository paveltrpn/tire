
#include "scene.h"

namespace tire::vk {

Scene::Scene( const std::filesystem::path &fname, uv_loop_t *loop )
    : tire::Scene{ fname, loop } {
}

}  // namespace tire::vk
