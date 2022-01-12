
#ifndef __algebra_h_
#define __algebra_h_

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];
typedef float mtrx3[9];
typedef float mtrx4[16];
typedef float qtnn[4];

#define f_eps 1.19209e-07

/*	multidimensional array mapping, array[i][j]
	row-wise (C, C++):
	(0	1)
	(2	3)

	column-wise (Fortran, Matlab):
	(0	2)
	(1	3)
*/

int32_t idRw(int32_t i, int32_t j, int32_t n);
int32_t idCw(int32_t i, int32_t j, int32_t n);

void    vec2Copy(const vec2 v, vec2 rt);
void    vec2Zero(vec2 v);
void    vec2Set(float x, float y, vec3 rt);
float 	vec2Lenght(const vec2 v);
void 	vec2Normalize(vec2 v);
void	vec2Scale(const vec2 v,const float scale, vec2 rt);
void	vec2Invert(vec2 v);
float	vec2Dot(const vec2 a, const vec2 b);
void	vec2Sum(const vec2 a, const vec2 b, vec2 rt);
void	vec2Sub(const vec2 a, const vec2 b, vec2 rt);

void    vec3Copy(const vec3 v, vec3 rt);
void    vec3Zero(vec3 v);
void 	vec3Set(float x, float y, float z, vec3 rt);
float 	vec3Lenght(const vec3 v);
void 	vec3Normalize(vec3 v);
void	vec3Scale(const vec3 v,const float scale, vec3 rt);
void	vec3Invert(vec3 v);
float	vec3Dot(const vec3 a, const vec3 b);
void	vec3Sum(const vec3 a, const vec3 b, vec3 rt);
void	vec3Sub(const vec3 a, const vec3 b, vec3 rt);
void	vec3Cross(const vec3 a, const vec3 b, vec3 rt);

void    vec4Copy(const vec4 v, vec4 rt);
void    vec4Zero(vec4 v);
void    vec4Set(float x, float y, float z, float w, vec4 rt);
float 	vec4Lenght(const vec4 v);
void 	vec4Normalize(vec4 v);
void	vec4Scale(const vec4 v,const float scale, vec4 rt);
void	vec4Invert(vec4 v);
float	vec4Dot(const vec4 a, const vec4 b);
void	vec4Sum(const vec4 a, const vec4 b, vec4 rt);
void	vec4Sub(const vec4 a, const vec4 b, vec4 rt);

void 	mtrx3Copy(const mtrx3 m, mtrx3 rt);
void    mtrx3Zero(mtrx3 m);
void    mtrx3Set(float a00, float a01, float a02,
                  float a10, float a11, float a12,
                  float a20, float a21, float a22,
                  mtrx3 rt);
void 	mtrx3SetEuler(const float yaw, const float pitch, const float roll, mtrx3 rt);
void 	mtrx3SetAxisAngl(const vec3 ax, const float phi, mtrx3 rt);
void	mtrx3Idtt(mtrx3 rt);
float	mtrx3Det(const mtrx3 m);
void	mtrx3Mult(const mtrx3 a, const mtrx3 b, mtrx3 rt);
void	mtrx3MultVec3(const mtrx3 m, const vec3 v, vec3 rt);
void	mtrx3LU(const mtrx3 m, mtrx3 lm, mtrx3 um);
void	mtrx3LDLT(const mtrx3 m, mtrx3 lm, vec3 dv);
void	mtrx3GetTranspose(const mtrx3 m, mtrx3 rt);
void	mtrx3TranposeSelf(mtrx3 m);
void    mtrx3GetInv(const mtrx3 m, mtrx3 rt);

void 	mtrx4Copy(const mtrx4 m, mtrx4 rt);
void    mtrx4Zero(mtrx4 m);
void    mtrx4Set(float a00, float a01, float a02, float a03,
                  float a10, float a11, float a12, float a13,
                  float a20, float a21, float a22, float a23,
                  float a30, float a31, float a32, float a33,
                  mtrx4 rt);
void	mtrx4Idtt(mtrx4 rt);
void	mtrx4LU(const mtrx4 m, mtrx4 lm, mtrx4 um);
void	mtrx4LDLT(const mtrx4 m, mtrx4 lm, vec4 dv);
void	mtrx4GetTranspose(const mtrx4 m, mtrx4 rt);
void	mtrx4TranposeSelf(mtrx4 m);
void    mtrx4GetInv(const mtrx4 m, mtrx4 rt);

#endif