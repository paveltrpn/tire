#version 450

layout( location = 0 ) in vec4 fragColor;
layout( location = 1 ) in vec3 vLighting;
layout( location = 2 ) in vec2 texCrd;

layout( location = 0 ) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler2D tex1;

void main() {
    vec3 color = texture(tex1,texCrd).xyz;
    outColor = fragColor*vec4(color, 1.0f);
    //outColor = fragColor*vec4(vLighting, 1.0f);
}
