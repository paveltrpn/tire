#version 450

layout(location = 0) out vec3 vWorldPos;

layout(push_constant) uniform PushConstants {
    mat4 projection;
    mat4 modelview;
} pc;

// Uniforms
layout(set = 0, binding = 0) uniform CameraBuffer {
    mat4 view;
    mat4 proj;
    vec3 cameraPos;
    float _padding;
} camera;

layout(set = 1, binding = 0) uniform GridBuffer {
    float gridSize;      // Distance between lines (e.g., 10.0)
    float lineThickness; // Width of lines in world units
    float maxRange;      // How far the grid is visible
    float zoomSensitivity;
} gridSettings;

vec3 positions[6] = vec3[](
        vec3( 10.0, -10.0, 0.0 ), vec3( 10.0, 10.0, 0.0 ), vec3( -10.0, 10.0, 0.0 ),
        vec3( -10.0, 10.0, 0.0 ), vec3( -10.0, -10.0, 0.0 ), vec3( 10.0, -10.0, 0.0 ) );

void main() {
    vec4 worldPos = vec4( positions[gl_VertexIndex], 1.0 );
    vWorldPos = worldPos.xyz;
    gl_Position = pc.projection * pc.modelview * worldPos;
}
