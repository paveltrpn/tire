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

const float LINE_LENGTH = 0.25;

// 24 line segments: 8 corners * 3 lines each (along box edges)
// Each line extends from corner toward adjacent corner along edge
vec3 lineVerts[48] = vec3[](
    // Corner 0 (-0.5,-0.5,-0.5) back-bottom-left - red
    vec3(-0.5, -0.5, -0.5), vec3(-0.5 + LINE_LENGTH, -0.5, -0.5),  // +X edge toward corner 1
    vec3(-0.5, -0.5, -0.5), vec3(-0.5, -0.5 + LINE_LENGTH, -0.5),  // +Y edge toward corner 3
    vec3(-0.5, -0.5, -0.5), vec3(-0.5, -0.5, -0.5 + LINE_LENGTH),  // +Z edge toward corner 4
    // Corner 1 (0.5,-0.5,-0.5) back-bottom-right - green
    vec3( 0.5, -0.5, -0.5), vec3( 0.5 - LINE_LENGTH, -0.5, -0.5),  // -X edge toward corner 0
    vec3( 0.5, -0.5, -0.5), vec3( 0.5, -0.5 + LINE_LENGTH, -0.5),  // +Y edge toward corner 2
    vec3( 0.5, -0.5, -0.5), vec3( 0.5, -0.5, -0.5 + LINE_LENGTH),  // +Z edge toward corner 5
    // Corner 2 (0.5,0.5,-0.5) back-top-right - blue
    vec3( 0.5,  0.5, -0.5), vec3( 0.5 - LINE_LENGTH,  0.5, -0.5),  // -X edge toward corner 3
    vec3( 0.5,  0.5, -0.5), vec3( 0.5,  0.5 - LINE_LENGTH, -0.5),  // -Y edge toward corner 1
    vec3( 0.5,  0.5, -0.5), vec3( 0.5,  0.5, -0.5 + LINE_LENGTH),  // +Z edge toward corner 6
    // Corner 3 (-0.5,0.5,-0.5) back-top-left - yellow
    vec3(-0.5,  0.5, -0.5), vec3(-0.5 + LINE_LENGTH,  0.5, -0.5),  // +X edge toward corner 2
    vec3(-0.5,  0.5, -0.5), vec3(-0.5,  0.5 - LINE_LENGTH, -0.5),  // -Y edge toward corner 0
    vec3(-0.5,  0.5, -0.5), vec3(-0.5,  0.5, -0.5 + LINE_LENGTH),  // +Z edge toward corner 7
    // Corner 4 (-0.5,-0.5,0.5) front-bottom-left - magenta
    vec3(-0.5, -0.5,  0.5), vec3(-0.5 + LINE_LENGTH, -0.5,  0.5),  // +X edge toward corner 5
    vec3(-0.5, -0.5,  0.5), vec3(-0.5, -0.5 + LINE_LENGTH,  0.5),  // +Y edge toward corner 7
    vec3(-0.5, -0.5,  0.5), vec3(-0.5, -0.5,  0.5 - LINE_LENGTH),  // -Z edge toward corner 0
    // Corner 5 (0.5,-0.5,0.5) front-bottom-right - cyan
    vec3( 0.5, -0.5,  0.5), vec3( 0.5 - LINE_LENGTH, -0.5,  0.5),  // -X edge toward corner 4
    vec3( 0.5, -0.5,  0.5), vec3( 0.5, -0.5 + LINE_LENGTH,  0.5),  // +Y edge toward corner 6
    vec3( 0.5, -0.5,  0.5), vec3( 0.5, -0.5,  0.5 - LINE_LENGTH),  // -Z edge toward corner 1
    // Corner 6 (0.5,0.5,0.5) front-top-right - orange
    vec3( 0.5,  0.5,  0.5), vec3( 0.5 - LINE_LENGTH,  0.5,  0.5),  // -X edge toward corner 7
    vec3( 0.5,  0.5,  0.5), vec3( 0.5,  0.5 - LINE_LENGTH,  0.5),  // -Y edge toward corner 5
    vec3( 0.5,  0.5,  0.5), vec3( 0.5,  0.5,  0.5 - LINE_LENGTH),  // -Z edge toward corner 2
    // Corner 7 (-0.5,0.5,0.5) front-top-left - gray
    vec3(-0.5,  0.5,  0.5), vec3(-0.5 + LINE_LENGTH,  0.5,  0.5),  // +X edge toward corner 6
    vec3(-0.5,  0.5,  0.5), vec3(-0.5,  0.5 - LINE_LENGTH,  0.5),  // -Y edge toward corner 4
    vec3(-0.5,  0.5,  0.5), vec3(-0.5,  0.5,  0.5 - LINE_LENGTH)   // -Z edge toward corner 3
);

void main() {
    gl_Position = pc.projection * pc.modelview * vec4(lineVerts[gl_VertexIndex], 1.0);
    fragColor = colors[gl_VertexIndex / 6];
}
