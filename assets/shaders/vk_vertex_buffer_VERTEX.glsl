#version 450

layout( location = 0 ) in vec3 inPosition;
layout( location = 0 ) out vec3 fragColor;

layout( push_constant ) uniform transformations_t {
    mat4 view;
}
transformations;

vec3 color = vec3( 1.0, 0.0, 0.0 );

void main() {
    gl_Position = transformations.view * vec4( inPosition, 1.0 );
    fragColor = color;
}
