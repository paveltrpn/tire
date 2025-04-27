
#version 450 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D frameTexture;

void main()
{ 
    FragColor = texture(frameTexture, TexCoords);
}