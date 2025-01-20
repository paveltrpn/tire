#version 450

layout( location = 0 ) out vec3 fragColor;
layout( location = 1 ) out vec3 vLighting;

layout( push_constant ) uniform transformations_t {
    mat4 view;
    mat4 rtn;
}
transformations;

vec3 positions[36] = vec3[](
    vec3( -0.5, 0.5, -0.5 ), vec3( 0.5, 0.5, 0.5 ), vec3( 0.5, 0.5, -0.5 ),
    vec3( 0.5, 0.5, 0.5 ), vec3( -0.5, -0.5, 0.5 ), vec3( 0.5, -0.5, 0.5 ),
    vec3( -0.5, 0.5, 0.5 ), vec3( -0.5, -0.5, -0.5 ), vec3( -0.5, -0.5, 0.5 ),
    vec3( 0.5, -0.5, -0.5 ), vec3( -0.5, -0.5, 0.5 ), vec3( -0.5, -0.5, -0.5 ),
    vec3( 0.5, 0.5, -0.5 ), vec3( 0.5, -0.5, 0.5 ), vec3( 0.5, -0.5, -0.5 ),
    vec3( -0.5, 0.5, -0.5 ), vec3( 0.5, -0.5, -0.5 ), vec3( -0.5, -0.5, -0.5 ),
    vec3( -0.5, 0.5, -0.5 ), vec3( -0.5, 0.5, 0.5 ), vec3( 0.5, 0.5, 0.5 ),
    vec3( 0.5, 0.5, 0.5 ), vec3( -0.5, 0.5, 0.5 ), vec3( -0.5, -0.5, 0.5 ),
    vec3( -0.5, 0.5, 0.5 ), vec3( -0.5, 0.5, -0.5 ), vec3( -0.5, -0.5, -0.5 ),
    vec3( 0.5, -0.5, -0.5 ), vec3( 0.5, -0.5, 0.5 ), vec3( -0.5, -0.5, 0.5 ),
    vec3( 0.5, 0.5, -0.5 ), vec3( 0.5, 0.5, 0.5 ), vec3( 0.5, -0.5, 0.5 ),
    vec3( -0.5, 0.5, -0.5 ), vec3( 0.5, 0.5, -0.5 ), vec3( 0.5, -0.5, -0.5 ) );

vec3 normals[36] = vec3[](
    vec3( -0.0, 1.0, -0.0 ), vec3( -0.0, 1.0, -0.0 ), vec3( -0.0, 1.0, -0.0 ),
    vec3( -0.0, -0.0, 1.0 ), vec3( -0.0, -0.0, 1.0 ), vec3( -0.0, -0.0, 1.0 ),
    vec3( -1.0, -0.0, -0.0 ), vec3( -1.0, -0.0, -0.0 ),
    vec3( -1.0, -0.0, -0.0 ), vec3( -0.0, -1.0, -0.0 ),
    vec3( -0.0, -1.0, -0.0 ), vec3( -0.0, -1.0, -0.0 ), vec3( 1.0, -0.0, -0.0 ),
    vec3( 1.0, -0.0, -0.0 ), vec3( 1.0, -0.0, -0.0 ), vec3( -0.0, -0.0, -1.0 ),
    vec3( -0.0, -0.0, -1.0 ), vec3( -0.0, -0.0, -1.0 ), vec3( -0.0, 1.0, -0.0 ),
    vec3( -0.0, 1.0, -0.0 ), vec3( -0.0, 1.0, -0.0 ), vec3( -0.0, -0.0, 1.0 ),
    vec3( -0.0, -0.0, 1.0 ), vec3( -0.0, -0.0, 1.0 ), vec3( -1.0, -0.0, -0.0 ),
    vec3( -1.0, -0.0, -0.0 ), vec3( -1.0, -0.0, -0.0 ),
    vec3( -0.0, -1.0, -0.0 ), vec3( -0.0, -1.0, -0.0 ),
    vec3( -0.0, -1.0, -0.0 ), vec3( 1.0, -0.0, -0.0 ), vec3( 1.0, -0.0, -0.0 ),
    vec3( 1.0, -0.0, -0.0 ), vec3( -0.0, -0.0, -1.0 ), vec3( -0.0, -0.0, -1.0 ),
    vec3( -0.0, -0.0, -1.0 ) );

vec3 color = vec3( 1.0, 0.0, 0.0 );
vec3 ambientLight = vec3( 0.3, 0.3, 0.3 );
vec3 lightcolor = vec3( 1.0, 1.0, 1.0 );

void main() {
    vec3 directionalVector = normalize( vec3( 0.0, 0.0, -5.0 ) );
    mat4 irtn = inverse( transformations.rtn );
    vec4 nrmTransformed = irtn * vec4( normals[gl_VertexIndex], 0.0 );
    float directional =
        max( dot( nrmTransformed.xyz, directionalVector ), 0.0 );
    vLighting = ambientLight + ( lightcolor * directional );
    vec4 rotated = transformations.rtn * vec4( positions[gl_VertexIndex], 1.0 );
    gl_Position = transformations.view * rotated;
    fragColor = color;
}
