#version 410

vec3 vLighting;
vec4 vColor;

void main(void) {
    gl_FragColor = vec4(vColor.rgb * vLighting, 1.0);
}
