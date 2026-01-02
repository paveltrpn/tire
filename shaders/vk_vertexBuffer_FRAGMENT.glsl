#version 450

layout( location = 0 ) in vec4 fragColor;
layout( location = 1 ) in vec3 vLighting;
layout( location = 2 ) in vec2 texCrd;

layout( location = 3 ) in vec3 EyePosition;
layout( location = 4 ) in vec3 DiffuseColor;
layout( location = 5 ) in vec2 TexCoord;
layout( location = 6 ) in vec3 Normal;
layout( location = 7 ) in vec3 FragmentPosition; 

layout( location = 0 ) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler2D tex1;

#define MAX_LIGHTS 15  
layout(set = 1, binding = 0) uniform OmniLight{
	vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} omniLights[MAX_LIGHTS];

void main() {
    vec3 color = texture(tex1,texCrd).xyz;
    outColor = fragColor*vec4(color, 1.0f);
    //outColor = fragColor*vec4(vLighting, 1.0f);
}
