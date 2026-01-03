#version 450

layout( location = 0 ) in vec3 vertex;
layout( location = 1 ) in vec3 normal;
layout( location = 2 ) in vec2 uv;

layout( push_constant ) uniform constants_t {
    dmat4 view;
    dvec3 eye;
} constants;

layout( location = 0 ) out vec3 EyePosition;
layout( location = 1 ) out vec3 DiffuseColor;
layout( location = 2 ) out vec2 TexCoord;
layout( location = 3 ) out vec3 Normal;
layout( location = 4 ) out vec3 FragmentPosition;

// For normal mapping
layout( location =  5) out vec2 FragmentTexcrd;

// Current scene lights count
layout( location =  6) flat out int LightsCount;

void main() {
    gl_Position = mat4(constants.view) * vec4( vertex, 1.0 );
    EyePosition = vec3(constants.eye.xyz);

    vec3 diffuseColor = vec3(0.5, 0.5, 0.5);
    int lightsCount = 5;

    FragmentPosition = vertex;
    FragmentTexcrd = uv;
    DiffuseColor = diffuseColor;
    TexCoord = uv;
    Normal = normal;
    LightsCount = lightsCount;
}
