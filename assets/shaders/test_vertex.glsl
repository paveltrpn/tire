#version 450 core
layout (location = 0) in vec3 pos;
out vec3 outColor;
uniform mat4 matrix;
uniform vec3 color;
void main() {
   outColor = color;
   gl_Position = matrix * vec4(pos, 1.0);
};