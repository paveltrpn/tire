#version 450

layout(location = 0) out vec3 vWorldPos;

layout(push_constant) uniform PushConstants {
    mat4 projection;
    mat4 modelview;
} pc;

vec3 positions[6] = vec3[](
        vec3( 10.0, -10.0, 0.0 ), vec3( 10.0, 10.0, 0.0 ), vec3( -10.0, 10.0, 0.0 ),
        vec3( -10.0, 10.0, 0.0 ), vec3( -10.0, -10.0, 0.0 ), vec3( 10.0, -10.0, 0.0 ) );

void main() {
    vec4 worldPos = vec4( positions[gl_VertexIndex], 1.0 );
    vWorldPos = worldPos.xyz;
    gl_Position = pc.projection * pc.modelview * worldPos;
}
