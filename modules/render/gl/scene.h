
#pragma once

#include <GL/gl.h>

#include "scene/scene.h"
#include "buffers/body_buffer.h"
#include "shader_storage.h"
#include "texture.h"

namespace tire::gl {

struct Scene final : tire::Scene {
    Scene( const std::filesystem::path &fname );

    void submit() override;
    void draw() override;
    void clean() override {};

private:
    std::vector<BodyBuffer> buffersList_;
    std::vector<gl::Texture> texturesList_{};

    gl::ShaderStorage shaderStorage_{};
};

}  // namespace tire::gl
