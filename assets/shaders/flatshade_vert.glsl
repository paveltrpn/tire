#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;
uniform mat4 view_matrix;
uniform vec3 color;

out vec3 newColor;
out vec3 vLighting;

void main()
{
    vec3 ambientLight = vec3(0.3, 0.3, 0.3);
    vec3 directionalLightColor = vec3(1, 1, 1);
    vec3 directionalVector = normalize(vec3(0, 1, -1));

    float directional = max(dot(nrm.xyz, directionalVector), 0.0);
    vLighting = ambientLight + (directionalLightColor * directional);

    gl_Position = view_matrix * vec4(pos, 1.0);

    newColor = color;
}
