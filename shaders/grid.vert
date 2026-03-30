
#version 450

layout(location = 0) out vec3 vWorldPos;

layout(push_constant) uniform PushConstants {
    mat4 projection;
    mat4 modelview;
} pc;

layout(set = 0, binding = 0) uniform PositionsBuffer {
    vec4 v[6];
} positions;

vec4 vertecies[6] = vec4[](
        vec4( 10.0, -10.0, 0.0, 1.0 ), vec4( 10.0,   10.0, 0.0, 1.0 ), vec4( -10.0, 10.0, 0.0, 1.0 ),
        vec4( -10.0, 10.0, 0.0, 1.0 ), vec4( -10.0, -10.0, 0.0, 1.0 ), vec4( 10.0, -10.0, 0.0, 1.0 ) );

void main() {
    // vec4 worldPos = positions.v[gl_VertexIndex];
    vec4 worldPos = vertecies[gl_VertexIndex];
    vWorldPos = worldPos.xyz;
    gl_Position = pc.projection * pc.modelview * worldPos;
}

