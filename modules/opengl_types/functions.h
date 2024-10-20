
#pragma once

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glcorearb.h>

namespace tire::opengl
{

// shaders
static PFNGLCREATEPROGRAMPROC glCreateProgram{ nullptr };
static PFNGLLINKPROGRAMPROC glLinkProgram{ nullptr };
static PFNGLUSEPROGRAMPROC glUseProgram{ nullptr };
static PFNGLSHADERSOURCEPROC glShaderSource{ nullptr };
static PFNGLCOMPILESHADERPROC glCompileShader{ nullptr };
static PFNGLCREATESHADERPROC glCreateShader{ nullptr };
static PFNGLGETSHADERIVPROC glGetShaderiv{ nullptr };
static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog{ nullptr };
static PFNGLDELETESHADERPROC glDeleteShader{ nullptr };
static PFNGLDELETEPROGRAMPROC glDeleteProgram{ nullptr };
static PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform{ nullptr };
static PFNGLGETPROGRAMIVPROC glGetProgramiv{ nullptr };
static PFNGLATTACHSHADERPROC glAttachShader{ nullptr };
static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog{ nullptr };
static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation{ nullptr };
static PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib{ nullptr };

static PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv{ nullptr };
static PFNGLUNIFORM3FVPROC glUniform3fv{ nullptr };

// VAO
static PFNGLGENBUFFERSPROC glGenBuffers{ nullptr };
static PFNGLGENVERTEXARRAYSPROC glGenVertexArrays{ nullptr };
static PFNGLBINDBUFFERPROC glBindBuffer{ nullptr };
static PFNGLBINDVERTEXARRAYPROC glBindVertexArray{ nullptr };
static PFNGLBUFFERDATAPROC glBufferData{ nullptr };
static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer{ nullptr };
static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray{ nullptr };
static PFNGLBUFFERSUBDATAPROC glBufferSubData{ nullptr };
static PFNGLDRAWARRAYSEXTPROC glDrawArrays{ nullptr };
static PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray{ nullptr };
static PFNGLDELETEBUFFERSPROC glDeleteBuffers{ nullptr };
static PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays{ nullptr };

void initOpenGLFunctions();

} // namespace tire::opengl
