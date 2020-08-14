
#include <cmath>

#include "plane.h"

plane_t::plane_t(const vec3_t &v, float d) {

}

plane_t::plane_t(const vec3_t &v1, const vec3_t &v2, const vec3_t &v3) {

}

vec3_t plane_ray_insct(const vec3_t &rs, const vec3_t &re) {
	return vec3_t();
}

bool point_in_trngl(const vec3_t &v1, 
					   const vec3_t &v2, 
					   const vec3_t &v3,
					   const vec3_t &point) {
	return false;
}

/*
	Взято из репозитория https://github.com/da-nie/SoftwareGraphicsLibrary/
	Статья на хабре - https://habr.com/ru/post/495664/
	
void CSGL::GetIntersectionPlaneAndLine(const SGLNVCTPoint& A,const SGLNVCTPoint& B,SGLNVCTPoint& new_point,float nx,float ny,float nz,float w)
{
 new_point=A;
 float ax=A.sGLVertex.X;
 float ay=A.sGLVertex.Y;
 float az=A.sGLVertex.Z;
 float au=A.sGLTexture.U;
 float av=A.sGLTexture.V;
 float ar=A.sGLColor.R;
 float ag=A.sGLColor.G;
 float ab=A.sGLColor.B;

 float bx=B.sGLVertex.X;
 float by=B.sGLVertex.Y;
 float bz=B.sGLVertex.Z;
 float bu=B.sGLTexture.U;
 float bv=B.sGLTexture.V;
 float br=B.sGLColor.R;
 float bg=B.sGLColor.G;
 float bb=B.sGLColor.B;

 float dx=bx-ax;
 float dy=by-ay;
 float dz=bz-az;
 float du=bu-au;
 float dv=bv-av;
 float dr=br-ar;
 float dg=bg-ag;
 float db=bb-ab;

 float top=(nx*ax)+(ny*ay)+(nz*az)+w;
 float bottom=(nx*dx)+(ny*dy)+(nz*dz);
 float time=-top/bottom;

 float vx=ax+time*dx;
 float vy=ay+time*dy;
 float vz=az+time*dz;
 float vu=au+time*du;
 float vv=av+time*dv;
 float r=ar+time*dr;
 float g=ag+time*dg;
 float b=ab+time*db;
 //äîáàâëÿåì íîâóþ òî÷êó
 SetVertexCoord(new_point.sGLVertex,vx,vy,vz);
 SetTextureCoord(new_point.sGLTexture,vu,vv);
 SetColorValue(new_point.sGLColor,(uint8_t)r,(uint8_t)g,(uint8_t)b);
}
*/