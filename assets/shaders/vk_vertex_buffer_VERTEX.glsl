#version 450

layout(location = 0) in vec3 inPosition;
layout( location = 0 ) out vec3 fragColor;

layout( push_constant ) uniform transformations_t {
    mat4 view;
    mat4 rtn;
}
transformations;

vec3 color = vec3( 1.0, 0.0, 0.0 );

void main() {
    vec4 rotated = transformations.rtn * vec4( inPosition, 1.0 );
    gl_Position = transformations.view * rotated;
    fragColor = color;
}
