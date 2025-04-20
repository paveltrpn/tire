
#pragma once

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glcorearb.h>

namespace tire::gl {

extern PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;

/* Creates a program object.
 * GLuint glCreateProgram(void)
 */
extern PFNGLCREATEPROGRAMPROC glCreateProgram;

/* Links a program object.
 * void glLinkProgram(GLuint program)
 */
extern PFNGLLINKPROGRAMPROC glLinkProgram;

/* Installs a program object as part of current rendering state.
 * void glUseProgram(GLuint program)
 */
extern PFNGLUSEPROGRAMPROC glUseProgram;

/*
 * Replaces the source code in a shader object.
 * void glShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
 */
extern PFNGLSHADERSOURCEPROC glShaderSource;

/* Compiles a shader object.
 * void glCompileShader( GLuint shader);
 */
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;

/* 
 * Attaches a shader object to a program object.
 * void glAttachShader(GLuint program, GLuint shader)
 */
extern PFNGLATTACHSHADERPROC glAttachShader;

extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib;
extern PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
extern PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLUNIFORM2FVPROC glUniform2fv;
extern PFNGLUNIFORM3FVPROC glUniform3fv;
extern PFNGLUNIFORM4FVPROC glUniform4fv;
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLUNIFORM1UIPROC glUniform1ui;

// GL_ARB_gpu_shader_fp64
extern PFNGLUNIFORMMATRIX2DVPROC glUniformMatrix2dv;
extern PFNGLUNIFORMMATRIX3DVPROC glUniformMatrix3dv;
extern PFNGLUNIFORMMATRIX4DVPROC glUniformMatrix4dv;
extern PFNGLUNIFORM2DVPROC glUniform2dv;
extern PFNGLUNIFORM3DVPROC glUniform3dv;
extern PFNGLUNIFORM4DVPROC glUniform4dv;
extern PFNGLUNIFORM1DPROC glUniform1d;

// Vertex buffers
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLDRAWARRAYSEXTPROC glDrawArrays;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
using PFNGLDRAWELEMENTSPROC = void ( * )( GLenum mode, GLsizei count,
                                          GLenum type, const void* indices );
extern PFNGLDRAWELEMENTSPROC glDrawElements;
extern PFNGLMAPBUFFERPROC glMapBuffer;
extern PFNGLUNMAPBUFFERPROC glUnmapBuffer;

// Textures
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
extern PFNGLCREATETEXTURESPROC glCreateTextures;
extern PFNGLTEXTURESTORAGE1DPROC glTextureStorage1D;
extern PFNGLTEXTURESTORAGE2DPROC glTextureStorage2D;
extern PFNGLTEXTURESTORAGE3DPROC glTextureStorage3D;
extern PFNGLTEXTURESUBIMAGE1DPROC glTextureSubImage1D;
extern PFNGLTEXTURESUBIMAGE2DPROC glTextureSubImage2D;
extern PFNGLTEXTURESUBIMAGE3DPROC glTextureSubImage3D;
extern PFNGLBINDTEXTUREUNITPROC glBindTextureUnit;
// using PFNGLDELETETEXTURESPROC = void ( * )( GLsizei n, const GLuint* textures );
// extern PFNGLDELETETEXTURESPROC glDeleteTextures;
extern PFNGLCREATESAMPLERSPROC glCreateSamplers;
extern PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri;
extern PFNGLSAMPLERPARAMETERFPROC glSamplerParameterf;
extern PFNGLBINDSAMPLERPROC glBindSampler;

void init();

}  // namespace tire::gl
