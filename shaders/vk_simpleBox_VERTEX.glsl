#version 450
layout( location = 0 ) out vec3 fragColor;
layout( location = 1 ) out vec3 vLighting;
layout( push_constant ) uniform transformations_t {
        mat4 view;
        mat4 model;
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
// Red cube
vec3 color = vec3( 1.0, 0.0, 0.0 );

vec3 ambientLight = vec3( 0.3, 0.3, 0.3 );
vec3 lightcolor = vec3( 1.0, 1.0, 1.0 );
vec3 lightpos = normalize( vec3( 10.0, 0.0, 10.0 ) );

void main() {
    vec4 nrmTransformed     = inverse(transpose( transformations.model )) * vec4( normals[gl_VertexIndex], 0.0 );
    float directional       = max( dot( nrmTransformed.xyz, lightpos ), 0.0 );
    vLighting               = ambientLight + ( lightcolor * directional );
    gl_Position             = transformations.view * transformations.model * vec4( positions[gl_VertexIndex], 1.0 );
    fragColor               = color;
}
