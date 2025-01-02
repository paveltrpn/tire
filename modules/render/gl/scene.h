
#pragma once

#include <filesystem>
#include <GL/gl.h>

#include "scene/scene.h"
#include "shader_storage.h"
#include "program.h"

namespace tire::gl {

struct SceneNodeBufferObjects final {
    GLuint vertexArray{};
    GLuint vertexBuffer{};
    GLuint elementsBuffer{};
    long long trianglesCount{};
};

struct Scene final : tire::Scene {
    Scene( const std::filesystem::path &fname );

    void initPrograms();

    void submit();
    void output();

private:
    std::vector<SceneNodeBufferObjects> buffersList_;

    gl::Program<ProgramColorTag> colorProgram_{};
    gl::Program<ProgramTextureTag> TextureProgram_{};

    gl::ShaderStorage shaderStorage_{};
};

}  // namespace tire::gl
