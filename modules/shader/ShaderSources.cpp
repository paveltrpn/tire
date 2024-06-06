
#include "ShaderSources.h"
#include "spdlog/spdlog.h"

namespace tire {

ShaderDatabase::ShaderDatabase() {
    vertexShaderSources_["default"] = R"foo(
void main() {
})foo";

    fragmentShaderSources_["default"] = R"foo(
void main() {
})foo";

    vertexShaderSources_["basic_color"] = R"foo(
#version 330 core
layout (location = 0) in vec3 pos;
out vec3 outColor;
uniform mat4 matrix;
uniform vec3 color;
void main() {
   outColor = color;
   gl_Position = matrix * vec4(pos, 1.0);
};)foo";

    fragmentShaderSources_["basic_color"] = R"foo(
#version 330 core
out vec4 FragColor;
in vec3 outColor;
void main() {
    FragColor = vec4(outColor, 0.5);
    //vec4 texColor = texture(texture1, texCoord);
    //if(texColor.a < 0.1)
    //     discard;
    //FragColor = texColor;
};)foo";
}

std::string ShaderDatabase::getVertexShader(const std::string &shaderId) {
    if (vertexShaderSources_.contains(shaderId)) {
        return vertexShaderSources_[shaderId];
    } else {
        spdlog::warn("can't find vertex shader source with id: {}", shaderId);
        return vertexShaderSources_["default"];
    }
}

std::string ShaderDatabase::getFragmentShader(const std::string &shaderId) {
    if (fragmentShaderSources_.contains(shaderId)) {
        return fragmentShaderSources_[shaderId];
    } else {
        spdlog::warn("can't find fragment shader source with id: {}", shaderId);
        return fragmentShaderSources_["default"];
    }
}

}  // namespace tire
