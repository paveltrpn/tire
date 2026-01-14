#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 txc;
layout (location = 2) in vec4 clr;

layout( location = 0 ) out vec2 TexCoord;
layout( location = 1 ) out vec4 Color;

layout( push_constant ) uniform constants_t {
    mat4 view;
} constants;

void main() {
#define SIZE 32
   float left     = -SIZE;
   float right    =  SIZE;
   float top      =  SIZE;
   float bottom   = -SIZE;
   float Znear    = -1;
   float Zfar     =  1;

   mat4 orthomatrix;
   /*
   orthomatrix[0].x = 2.0/(right-left);
   orthomatrix[0].y = 0;
   orthomatrix[0].z = 0;
   orthomatrix[0].w = 0;

   orthomatrix[1].x = 0;
   orthomatrix[1].y = 2.0/(top-bottom);
   orthomatrix[1].z = 0;
   orthomatrix[1].w = 0;

   orthomatrix[2].x = 0;
   orthomatrix[2].y = 0;
   orthomatrix[2].z = 2.0/(Zfar-Znear);
   orthomatrix[2].w = 0;

   orthomatrix[3].x = -(right+left)/(right-left);
   orthomatrix[3].y = -(top+bottom)/(top-bottom);
   orthomatrix[3].z = -(Zfar+Znear)/(Zfar-Znear);
   orthomatrix[3].w = 1;
   */

   
   orthomatrix[0][0] = 2.0/(right-left);
   orthomatrix[1][0] = 0;
   orthomatrix[2][0] = 0;
   orthomatrix[3][0] = -(right+left)/(right-left);

   orthomatrix[0][1] = 0;
   orthomatrix[1][1] = 2.0/(top-bottom);
   orthomatrix[2][1] = 0;
   orthomatrix[3][1] = -(top+bottom)/(top-bottom);

   orthomatrix[0][2] = 0;
   orthomatrix[1][2] = 0;
   orthomatrix[2][2] = 2.0/(Zfar-Znear);
   orthomatrix[3][2] = -(Zfar+Znear)/(Zfar-Znear);

   orthomatrix[0][3] = 0;
   orthomatrix[1][3] = 0;
   orthomatrix[2][3] = 0;
   orthomatrix[3][3] = 1;
   

   /*
   orthomatrix[0][0] = 1;
   orthomatrix[1][0] = 0;
   orthomatrix[2][0] = 0;
   orthomatrix[3][0] = 0;

   orthomatrix[0][1] = 0;
   orthomatrix[1][1] = 1;
   orthomatrix[2][1] = 0;
   orthomatrix[3][1] = 0;

   orthomatrix[0][2] = 0;
   orthomatrix[1][2] = 0;
   orthomatrix[2][2] = 1;
   orthomatrix[3][2] = 0;

   orthomatrix[0][3] = 0;
   orthomatrix[1][3] = 0;
   orthomatrix[2][3] = 0;
   orthomatrix[3][3] = 1;
   */
   gl_Position = constants.view*vec4(pos, 1.0);
   TexCoord = txc;
   // Color = vec4(clr, 1.0);
   Color = vec4(0.5, 0.5, 0.5, 1.0);
}
