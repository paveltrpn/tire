
#pragma once

#include <GL/gl.h>

#include "scene/scene.h"
#include "buffers/object_buffer.h"
#include "shader_storage.h"

namespace tire::gl {

struct Scene final : tire::Scene {
    Scene( const std::filesystem::path &fname );

    void submit();
    void draw();

private:
    std::vector<ObjectBuffer> buffersList_;
    gl::ShaderStorage shaderStorage_{};
};

}  // namespace tire::gl
