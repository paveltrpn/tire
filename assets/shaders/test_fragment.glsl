#version 450 core
out vec4 FragColor;
in vec3 outColor;
void main() {
    FragColor = vec4(outColor, 0.5);
    //vec4 texColor = texture(texture1, texCoord);
    //if(texColor.a < 0.1)
    //     discard;
    //FragColor = texColor;
};