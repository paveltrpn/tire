
#include <GLFW/glfw3.h>

#include "Config.h"
#include "Render.h"

namespace tire {

Render::Render(const tire::Config& config) : config_{ config } {
    doublebuffer_ = config_.get<bool>("doublebuffer");
}

}  // namespace tire
