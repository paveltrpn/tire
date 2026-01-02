#version 450

layout( location = 0 ) in vec3 vertex;
layout( location = 1 ) in vec3 normal;
layout( location = 2 ) in vec2 uv;

layout( location = 0 ) out vec4 fragColor;
layout( location = 1 ) out vec3 vLighting;
layout( location = 2 ) out vec2 texCrd;

layout( push_constant ) uniform constants_t {
    dmat4 view;
    dvec3 eye;
} constants;

void main() {
    vec3 ambientLight = vec3(0.3, 0.3, 0.3);
    vec3 lightpos = vec3(1.0, 1.0, 0.0);
    vec3 lightcolor = vec3(1.0, 1.0, 1.0);

    vec3 directionalVector = normalize(lightpos);

    float directional = max(dot(normal.xyz, directionalVector), 0.0);
    vLighting = ambientLight + (lightcolor * directional);

    gl_Position = mat4(constants.view) * vec4( vertex, 1.0 );

    fragColor.x = uv.x;
    fragColor.y = uv.y;
    fragColor.z = 0.5;
    fragColor.w = 1.0;

    texCrd = uv;
}
