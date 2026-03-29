#version 450

// // 1. Create a Vertex Buffer for a large plane centered at (0,0,0)
// //    Vertices: (-1000, 0, -1000), (1000, 0, -1000), (1000, 0, 1000)...
// //    We rely on the Fragment Shader to discard fragments outside the "Infinite" illusion.

// // 2. Setup Uniforms
// GridBuffer gridData;
// gridData.gridSize = 10.0f;          // 10 units between lines
// gridData.lineThickness = 0.5f;      // 0.5 units thick
// gridData.maxRange = 2000.0f;        // Grid visible up to 2km
// gridData.zoomSensitivity = 0.5f;    // 0 = static thickness, 1 = thickness grows with distance
// vkUpdateDescriptorSets(...);

// // 3. Setup Push Constants (Colors)
// PushConstants push;
// push.colorMajor = {1.0f, 1.0f, 1.0f}; // White lines every 50 units
// push.colorMinor = {0.5f, 0.5f, 0.5f}; // Grey lines every 10 units
// push.majorDivisor = 5;
// vkCmdPushConstants(...);

// // 4. Render
// vkCmdBindPipeline(...);
// vkCmdBindDescriptorSets(...);
// vkCmdDraw(...); // Draw your large plane

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
        vec3( 0.5, -0.5, 0.0 ), vec3( 0.5, 0.5, 0.0 ), vec3( -0.5, 0.5, 0.0 ),
        vec3( -0.5, 0.5, 0.0 ), vec3( -0.5, -0.5, 0.0 ), vec3( 0.5, -0.5, 0.0 ) );

void main() {
    vec4 worldPos = vec4( positions[gl_VertexIndex], 1.0 );
    vWorldPos = worldPos.xyz;
    gl_Position = pc.projection * pc.modelview * worldPos;
}
