
#pragma once

#include <GL/gl.h>

#include "scene/physics_scene.h"
#include "scene/scene.h"
#include "buffers/body_buffer.h"
#include "shader_storage.h"

namespace tire::gl {

struct Scene final : tire::Scene {
    Scene( const std::filesystem::path &fname );

    void submit();
    void draw();

private:
    std::vector<BodyBuffer> buffersList_;
    gl::ShaderStorage shaderStorage_{};
};

}  // namespace tire::gl
