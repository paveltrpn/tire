#version 450

layout( location = 0 ) in vec3 EyePosition;
layout( location = 1 ) in vec3 DiffuseColor;
layout( location = 2 ) in vec2 TexCoord;
layout( location = 3 ) in vec3 Normal;
layout( location = 4 ) in vec3 FragmentPosition; 
layout( location = 5 ) in vec2 FragmentTexcrd;  
layout( location = 6 ) flat in int LightsCount;

layout( location = 0 ) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler2D tex1;

struct OmniLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

#define MAX_LIGHTS 15  
layout(set = 1, binding = 1) uniform OmniLights {
    OmniLight data[MAX_LIGHTS];
} omniLights;

// PBR-maps for roughness (and metallic) are usually stored in non-linear
// color space (sRGB), so we use these functions to convert into linear RGB.
vec3 PowVec3(vec3 v, float p)
{
    return vec3(pow(v.x, p), pow(v.y, p), pow(v.z, p));
}

const float gamma = 0.81;
vec3 ToLinear(vec3 v) { return PowVec3(v, gamma); }
vec3 ToSRGB(vec3 v)   { return PowVec3(v, 1.0/gamma); }

vec3 CalcPointLight(OmniLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    

    // combine results
    vec3 ambient  = light.ambient * vec3(texture(tex1, TexCoord));

     // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(tex1, TexCoord));

     // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float materialShininess = 128;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    //vec3 specular = light.specular * spec * vec3(texture(roughnessTexture, TexCoord));


    ambient  *= attenuation;
    diffuse  *= attenuation;
    //specular *= attenuation;

    //return (diffuse + ambient + specular);
    return (diffuse + ambient);
} 

void main() {
    // Pass alreaady normalized?
    vec3 norm = normalize(Normal);

    // Get normal from normal map, but we need tangent space...
    //vec3 norm = texture(normalmapTexture, FragmentTexcrd).rgb;
    //norm = normalize(norm * 2.0 - 1.0);  

    vec3 viewDir = normalize(EyePosition - FragmentPosition);

    vec3 result = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < LightsCount; i++)
        result += CalcPointLight(omniLights.data[i], norm, FragmentPosition, viewDir);    
    
    // outColor = vec4(DiffuseColor.rgb*result, 1.0);
    vec3 color  = vec3(DiffuseColor.rgb*result);

    outColor = vec4( ToSRGB(color), 1.0 );
}
