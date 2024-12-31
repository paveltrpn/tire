
#pragma once

#include <filesystem>
#include <GL/gl.h>

#include "scene/scene.h"

namespace tire::gl {

struct SceneNodeBufferObjects final {
    GLuint vertexArray{};
    GLuint vertexBuffer{};
    GLuint elementsBuffer{};
    long long trianglesCount{};
};

struct Scene final : tire::Scene {
    Scene( const std::filesystem::path &fname );

    void submit();
    void output();

private:
    std::vector<SceneNodeBufferObjects> buffersList_;
};

}  // namespace tire::gl
