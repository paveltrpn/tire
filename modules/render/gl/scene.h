
#pragma once

#include <filesystem>
#include <GL/gl.h>
#include "uv.h"

#include "scene/scene.h"
#include "shader_storage.h"

namespace tire::gl {

struct Scene final : tire::Scene {
    Scene( const std::filesystem::path &fname );

    void submit();
    void draw();

private:
    // OpenGL side object entities data
    struct SceneNodeBufferObjects final {
        GLuint vertexArray{};
        GLuint vertexBuffer{};
        GLuint normalBuffer{};
        GLuint texcrdBuffer{};
        GLuint texture{};
    };

    std::vector<SceneNodeBufferObjects> buffersList_;
    gl::ShaderStorage shaderStorage_{};
};

}  // namespace tire::gl
