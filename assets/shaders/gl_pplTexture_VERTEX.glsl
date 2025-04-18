#version 450 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexNrm;
layout (location = 2) in vec2 vertexTxc;

uniform mat4 viewMatrix;
uniform vec3 eyePosition;
uniform vec3 diffuseColor;
uniform int lightsCount;

out vec3 EyePosition;
out vec3 DiffuseColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragmentPosition;  
out flat uint LightsCount;

void main()
{
    gl_Position = viewMatrix * vec4(vertexPos, 1.0);
    EyePosition = eyePosition;
    FragmentPosition = vertexPos;
    DiffuseColor = diffuseColor;
    TexCoord = vertexTxc;
    Normal = vertexNrm;
    LightsCount = lightsCount;
}
