
#pragma once

#include <unordered_map>

#include <GL/gl.h>

#include "scene/scene.h"
#include "buffers/body_buffer.h"
#include "program.h"
#include "texture.h"
#include "material.h"

namespace tire::gl {

struct Scene final : tire::Scene {
    Scene( const std::filesystem::path &fname );

    void submit() override;
    void draw() override;
    void clean() override {};

private:
    std::vector<BodyBuffer> buffersList_;
    gl::Program pplTexture{};
    std::unordered_map<std::string, TextureSet> materialSet_{};
};

}  // namespace tire::gl
