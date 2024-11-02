
#pragma once

#include <iostream>
#include <string>
#include <map>

#include "log/log.h"

namespace tire
{

struct ShaderDatabase final {
    ShaderDatabase() {
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

    std::string getVertexShader( const std::string &shaderId ) {
        if ( vertexShaderSources_.contains( shaderId ) ) {
            return vertexShaderSources_[shaderId];
        } else {
            log::warning( "can't find vertex shader source with id: {}", shaderId );
            return vertexShaderSources_["default"];
        }
    }
    std::string getFragmentShader( const std::string &shaderId ) {
        if ( fragmentShaderSources_.contains( shaderId ) ) {
            return fragmentShaderSources_[shaderId];
        } else {
            log::warning( "can't find fragment shader source with id: {}", shaderId );
            return fragmentShaderSources_["default"];
        }
    }

private:
    std::map<std::string, std::string> vertexShaderSources_;
    std::map<std::string, std::string> fragmentShaderSources_;
};

} // namespace tire
