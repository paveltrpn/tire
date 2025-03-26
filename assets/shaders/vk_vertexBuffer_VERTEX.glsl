#version 450

layout( location = 0 ) in vec3 inPositionOne;
layout( location = 0 ) out vec3 fragColor;

layout( push_constant ) uniform constants_t {
    mat4 view;
    vec3 color;
} constants;

void main() {
    gl_Position = constants.view * vec4( inPositionOne, 1.0 );
    // WHAT!!!
    gl_Position.z /= gl_Position.w;
    fragColor = constants.color;
}
