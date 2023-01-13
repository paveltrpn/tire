
#ifndef __glext_local_h__
#define __glext_local_h__

#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>

#define GL_SHADING_LANGUAGE_VERSION 0x8B8C

void *GetAnyGLFuncAddress(const char *name)
{
	void *p = (void *)wglGetProcAddress(name);
	if (p == 0 || (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) || (p == (void*)-1))
	{
		HMODULE module = LoadLibraryA("opengl32.dll");
		p = (void *)GetProcAddress(module, name);
	}
	return p;
}

#define GLAPIENTRY __stdcall

typedef GLuint(GLAPIENTRY * PFNGLCREATEPROGRAMPROC) (void);
PFNGLCREATEPROGRAMPROC glCreateProgram = (PFNGLCREATEPROGRAMPROC)GetAnyGLFuncAddress("glCreateProgram");

typedef void (GLAPIENTRY * PFNGLUSEPROGRAM)(GLuint program);
PFNGLUSEPROGRAM glUseProgram = (PFNGLUSEPROGRAM)GetAnyGLFuncAddress("glUseProgram");

#endif
