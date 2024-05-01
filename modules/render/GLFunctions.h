
#ifndef __glfunctions_h__
#define __glfunctions_h__

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glcorearb.h>

namespace tire {

extern PFNGLCREATEPROGRAMPROC glCreateProgram;

void initGL();

struct GLFunctions {
        void initGLFunctions();

        // shaders
        PFNGLCREATEPROGRAMPROC CreateProgram{ nullptr };
        PFNGLLINKPROGRAMPROC LinkProgram{ nullptr };
        PFNGLUSEPROGRAMPROC UseProgram{ nullptr };
        PFNGLSHADERSOURCEPROC ShaderSource{ nullptr };
        PFNGLCOMPILESHADERPROC CompileShader{ nullptr };
        PFNGLCREATESHADERPROC CreateShader{ nullptr };
        PFNGLGETSHADERIVPROC GetShaderiv{ nullptr };
        PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog{ nullptr };
        PFNGLDELETESHADERPROC DeleteShader{ nullptr };
        PFNGLDELETEPROGRAMPROC DeleteProgram{ nullptr };
        PFNGLGETACTIVEUNIFORMPROC GetActiveUniform{ nullptr };
        PFNGLGETPROGRAMIVPROC GetProgramiv{ nullptr };
        PFNGLATTACHSHADERPROC AttachShader{ nullptr };
        PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog{ nullptr };
        PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation{ nullptr };
        PFNGLUNIFORMMATRIX4FVPROC UniformMatrix4fv{ nullptr };
        PFNGLGETACTIVEATTRIBPROC GetActiveAttrib{ nullptr };

        // VAO
        PFNGLGENBUFFERSPROC GenBuffers{ nullptr };
        PFNGLGENVERTEXARRAYSPROC GenVertexArrays{ nullptr };
        PFNGLBINDBUFFERPROC BindBuffer{ nullptr };
        PFNGLBINDVERTEXARRAYPROC BindVertexArray{ nullptr };
        PFNGLBUFFERDATAPROC BufferData{ nullptr };
        PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer{ nullptr };
        PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray{ nullptr };
        PFNGLBUFFERSUBDATAPROC BufferSubData{ nullptr };
        PFNGLDRAWARRAYSEXTPROC DrawArrays{ nullptr };
        PFNGLDISABLEVERTEXATTRIBARRAYPROC DisableVertexAttribArray{ nullptr };
        PFNGLDELETEBUFFERSPROC DeleteBuffers{ nullptr };
        PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays{ nullptr };
};

}  // namespace tire

#endif
