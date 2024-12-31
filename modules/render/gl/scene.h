
#pragma once

#include <filesystem>
#include <GL/gl.h>

#include "scene/scene.h"

namespace tire::gl {

struct SceneNodeBufferObjects final {
    GLuint bufferObject{};
    GLuint vertexObject{};
    GLuint elementsObject{};
    long long trianglesCount{};
};

struct Scene final : tire::Scene {
    Scene( const std::filesystem::path &fname );

    void output();

private:
    std::vector<SceneNodeBufferObjects> buffersList_;
};

}  // namespace tire::gl
