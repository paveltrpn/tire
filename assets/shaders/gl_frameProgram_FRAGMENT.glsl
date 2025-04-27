
#version 450 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D frameTexture;

uniform vec2 mouseFocusPoint;

vec4 chromaticAbberations() {
    float redOffset   =  0.009;
    float greenOffset =  0.006;
    float blueOffset  = -0.006;

    vec2 texSize  = textureSize(frameTexture, 0).xy;
    vec2 texCoord = gl_FragCoord.xy / texSize;

    vec2 direction = texCoord - mouseFocusPoint;

    vec4 ret;

    ret.r  = texture(frameTexture, TexCoords + (direction * vec2(redOffset  ))).r;
    ret.g  = texture(frameTexture, TexCoords + (direction * vec2(greenOffset))).g;
    ret.ba = texture(frameTexture, TexCoords + (direction * vec2(blueOffset ))).ba;

    return ret;
}

void main()
{ 
    FragColor = chromaticAbberations();
    // FragColor = texture(frameTexture, TexCoords);
}