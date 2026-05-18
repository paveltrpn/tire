#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(push_constant) uniform PushConstants {
    mat4 projection;
    mat4 modelview;
} pc;

layout(location = 0) out vec3 fragColor;

out gl_PerVertex {
    vec4 gl_Position;
};

// 8 corner positions (normalized -0.5 to +0.5)
vec3 corners[8] = vec3[](
    vec3(-0.5, -0.5, -0.5),  // 0: back-bottom-left
    vec3( 0.5, -0.5, -0.5),  // 1: back-bottom-right
    vec3( 0.5,  0.5, -0.5),  // 2: back-top-right
    vec3(-0.5,  0.5, -0.5),  // 3: back-top-left
    vec3(-0.5, -0.5,  0.5),  // 4: front-bottom-left
    vec3( 0.5, -0.5,  0.5),  // 5: front-bottom-right
    vec3( 0.5,  0.5,  0.5),  // 6: front-top-right
    vec3(-0.5,  0.5,  0.5)   // 7: front-top-left
);

vec3 colors[8] = vec3[](
    vec3(1.0, 0.0, 0.0),     // 0: red
    vec3(0.0, 1.0, 0.0),     // 1: green
    vec3(0.0, 0.0, 1.0),     // 2: blue
    vec3(1.0, 1.0, 0.0),     // 3: yellow
    vec3(1.0, 0.0, 1.0),     // 4: magenta
    vec3(0.0, 1.0, 1.0),     // 5: cyan
    vec3(1.0, 0.5, 0.0),     // 6: orange
    vec3(0.5, 0.5, 0.5)      // 7: gray
);

// 24 line segments: 8 corners * 3 lines each
// Each line has 2 vertices
vec3 lineVerts[48] = vec3[](
    // Corner 0 (red) - back-bottom-left
    vec3(-0.5, -0.5, -0.5), vec3( 0.0, -0.5, -0.5),  // X axis
    vec3(-0.5, -0.5, -0.5), vec3(-0.5,  0.0, -0.5),  // Y axis
    vec3(-0.5, -0.5, -0.5), vec3(-0.5, -0.5,  0.0),  // Z axis
    // Corner 1 (green) - back-bottom-right
    vec3( 0.5, -0.5, -0.5), vec3( 0.0, -0.5, -0.5),  // X axis
    vec3( 0.5, -0.5, -0.5), vec3( 0.5,  0.0, -0.5),  // Y axis
    vec3( 0.5, -0.5, -0.5), vec3( 0.5, -0.5,  0.0),  // Z axis
    // Corner 2 (blue) - back-top-right
    vec3( 0.5,  0.5, -0.5), vec3( 0.0,  0.5, -0.5),  // X axis
    vec3( 0.5,  0.5, -0.5), vec3( 0.5,  1.0, -0.5),  // Y axis
    vec3( 0.5,  0.5, -0.5), vec3( 0.5,  0.5,  0.0),  // Z axis
    // Corner 3 (yellow) - back-top-left
    vec3(-0.5,  0.5, -0.5), vec3(-1.0,  0.5, -0.5),  // X axis
    vec3(-0.5,  0.5, -0.5), vec3(-0.5,  1.0, -0.5),  // Y axis
    vec3(-0.5,  0.5, -0.5), vec3(-0.5,  0.5,  0.0),  // Z axis
    // Corner 4 (magenta) - front-bottom-left
    vec3(-0.5, -0.5,  0.5), vec3(-1.0, -0.5,  0.5),  // X axis
    vec3(-0.5, -0.5,  0.5), vec3(-0.5,  0.0,  0.5),  // Y axis
    vec3(-0.5, -0.5,  0.5), vec3(-0.5, -0.5,  1.0),  // Z axis
    // Corner 5 (cyan) - front-bottom-right
    vec3( 0.5, -0.5,  0.5), vec3( 0.0, -0.5,  0.5),  // X axis
    vec3( 0.5, -0.5,  0.5), vec3( 0.5,  0.0,  0.5),  // Y axis
    vec3( 0.5, -0.5,  0.5), vec3( 0.5, -0.5,  1.0),  // Z axis
    // Corner 6 (orange) - front-top-right
    vec3( 0.5,  0.5,  0.5), vec3( 0.0,  0.5,  0.5),  // X axis
    vec3( 0.5,  0.5,  0.5), vec3( 0.5,  1.0,  0.5),  // Y axis
    vec3( 0.5,  0.5,  0.5), vec3( 0.5,  0.5,  1.0),  // Z axis
    // Corner 7 (gray) - front-top-left
    vec3(-0.5,  0.5,  0.5), vec3(-1.0,  0.5,  0.5),  // X axis
    vec3(-0.5,  0.5,  0.5), vec3(-0.5,  1.0,  0.5),  // Y axis
    vec3(-0.5,  0.5,  0.5), vec3(-0.5,  0.5,  1.0)   // Z axis
);

void main() {
    gl_Position = pc.projection * pc.modelview * vec4(lineVerts[gl_VertexIndex], 1.0);
    fragColor = colors[gl_VertexIndex / 6];
}
