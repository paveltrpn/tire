
#include "GLFunctions.h"
#include <GL/glcorearb.h>
#include <GL/glext.h>

namespace tire {

void GLFunctions::initGLFunctions() {
    // shaders
    CreateProgram = reinterpret_cast<PFNGLCREATEPROGRAMPROC>(
      glXGetProcAddress(reinterpret_cast<const GLubyte *>("glCreateProgram")));

    LinkProgram
      = reinterpret_cast<PFNGLLINKPROGRAMPROC>(glXGetProcAddress((const GLubyte *)"glLinkProgram"));

    UseProgram
      = reinterpret_cast<PFNGLUSEPROGRAMPROC>(glXGetProcAddress((const GLubyte *)"glUseProgram"));

    ShaderSource = reinterpret_cast<PFNGLSHADERSOURCEPROC>(
      glXGetProcAddress((const GLubyte *)"glShaderSource"));

    CompileShader = reinterpret_cast<PFNGLCOMPILESHADERPROC>(
      glXGetProcAddress((const GLubyte *)"glCompileShader"));

    CreateShader = reinterpret_cast<PFNGLCREATESHADERPROC>(
      glXGetProcAddress((const GLubyte *)"glCreateShader"));

    GetShaderiv
      = reinterpret_cast<PFNGLGETSHADERIVPROC>(glXGetProcAddress((const GLubyte *)"glGetShaderiv"));

    GetShaderInfoLog = reinterpret_cast<PFNGLGETSHADERINFOLOGPROC>(
      glXGetProcAddress((const GLubyte *)"glGetShaderInfoLog"));

    DeleteShader = reinterpret_cast<PFNGLDELETESHADERPROC>(
      glXGetProcAddress((const GLubyte *)"glDeleteShader"));

    DeleteProgram = reinterpret_cast<PFNGLDELETEPROGRAMPROC>(
      glXGetProcAddress((const GLubyte *)"glDeleteProgram"));

    GetActiveUniform = reinterpret_cast<PFNGLGETACTIVEUNIFORMPROC>(
      glXGetProcAddress((const GLubyte *)"glGetActiveUniform"));

    GetProgramiv = reinterpret_cast<PFNGLGETPROGRAMIVPROC>(
      glXGetProcAddress((const GLubyte *)"glGetProgramiv"));

    AttachShader = reinterpret_cast<PFNGLATTACHSHADERPROC>(
      glXGetProcAddress((const GLubyte *)"glAttachShader"));

    GetProgramInfoLog = reinterpret_cast<PFNGLGETPROGRAMINFOLOGPROC>(
      glXGetProcAddress((const GLubyte *)"glGetProgramInfoLog"));

    GetUniformLocation = reinterpret_cast<PFNGLGETUNIFORMLOCATIONPROC>(
      glXGetProcAddress((const GLubyte *)"glGetUniformLocation"));

    UniformMatrix4fv = reinterpret_cast<PFNGLUNIFORMMATRIX4FVPROC>(
      glXGetProcAddress((const GLubyte *)"glUniformMatrix4fv"));

    // VAO
    GenBuffers = reinterpret_cast<PFNGLGENBUFFERSPROC>(
      glXGetProcAddress(reinterpret_cast<const GLubyte *>("glGenBuffers")));

    GenVertexArrays = reinterpret_cast<PFNGLGENVERTEXARRAYSPROC>(
      glXGetProcAddress(reinterpret_cast<const GLubyte *>("glGenVertexArrays")));

    BindBuffer = reinterpret_cast<PFNGLBINDBUFFERPROC>(
      glXGetProcAddress(reinterpret_cast<const GLubyte *>("glBindBuffer")));

    BindVertexArray = reinterpret_cast<PFNGLBINDVERTEXARRAYPROC>(
      glXGetProcAddress(reinterpret_cast<const GLubyte *>("glBindVertexArray")));

    BufferData = reinterpret_cast<PFNGLBUFFERDATAPROC>(
      glXGetProcAddress(reinterpret_cast<const GLubyte *>("glBufferData")));

    VertexAttribPointer = reinterpret_cast<PFNGLVERTEXATTRIBPOINTERPROC>(
      glXGetProcAddress(reinterpret_cast<const GLubyte *>("glVertexAttribPointer")));

    EnableVertexAttribArray = reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYPROC>(
      glXGetProcAddress(reinterpret_cast<const GLubyte *>("glEnableVertexAttribArray")));

    BufferSubData = reinterpret_cast<PFNGLBUFFERSUBDATAPROC>(
      glXGetProcAddress(reinterpret_cast<const GLubyte *>("glBufferSubData")));

    DrawArrays = reinterpret_cast<PFNGLDRAWARRAYSEXTPROC>(
      glXGetProcAddress(reinterpret_cast<const GLubyte *>("glDrawArrays")));

    DisableVertexAttribArray = reinterpret_cast<PFNGLDISABLEVERTEXATTRIBARRAYPROC>(
      glXGetProcAddress(reinterpret_cast<const GLubyte *>("glDisableVertexAttribArray")));

    DeleteBuffers = reinterpret_cast<PFNGLDELETEBUFFERSPROC>(
      glXGetProcAddress(reinterpret_cast<const GLubyte *>("glDeleteBuffers")));

    DeleteVertexArrays = reinterpret_cast<PFNGLDELETEVERTEXARRAYSPROC>(
      glXGetProcAddress(reinterpret_cast<const GLubyte *>("glDeleteVertexArrays")));
}

}  // namespace tire
