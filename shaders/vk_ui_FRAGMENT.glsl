#version 450 core

layout( location = 0 ) in vec2 TexCoord;
layout( location = 1 ) in vec4 Color;

layout( location = 0 ) out vec4 FragColor;

layout(set = 0, binding = 0) uniform sampler2D fontTexture;

layout( push_constant ) uniform Constants {
    layout(offset = 16) ivec4 texureFlag;
} constants;

void main() {
    vec4 c;

    if (constants.texureFlag.x == 1) {
        c = texture(fontTexture, TexCoord) * Color;
    } else {
        c =  Color;
    }
    FragColor = c;
}
