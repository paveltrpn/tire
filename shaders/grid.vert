
#version 450

layout(location = 0) out vec3 vWorldPos;

layout(push_constant) uniform PushConstants {
    mat4 projection;
    mat4 modelview;
} pc;

layout(set = 0, binding = 0) uniform PositionsBuffer {
    vec4 v[6];
} positions;

vec3 vertecies[6] = vec3[](
        vec3( 10.0, -10.0, 0.0 ), vec3( 10.0, 10.0, 0.0 ), vec3( -10.0, 10.0, 0.0 ),
        vec3( -10.0, 10.0, 0.0 ), vec3( -10.0, -10.0, 0.0 ), vec3( 10.0, -10.0, 0.0 ) );

void main() {
    // vec4 worldPos = positions.v[gl_VertexIndex];
    vec4 worldPos = vec4(vertecies[gl_VertexIndex], 1.0);
    vWorldPos = worldPos.xyz;
    gl_Position = pc.projection * pc.modelview * worldPos;
}

