
#ifndef __glfunctions_h__
#define __glfunctions_h__

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glcorearb.h>

namespace tire {

struct GLFunctions {
        void initGLFunctions();

        // shaders
        PFNGLCREATEPROGRAMPROC CreateProgram;
        PFNGLLINKPROGRAMPROC LinkProgram;
        PFNGLUSEPROGRAMPROC UseProgram;
        PFNGLSHADERSOURCEPROC ShaderSource;
        PFNGLCOMPILESHADERPROC CompileShader;
        PFNGLCREATESHADERPROC CreateShader;
        PFNGLGETSHADERIVPROC GetShaderiv;
        PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog;
        PFNGLDELETESHADERPROC DeleteShader;
        PFNGLDELETEPROGRAMPROC DeleteProgram;
        PFNGLGETACTIVEUNIFORMPROC GetActiveUniform;

        // VAO
        PFNGLGENBUFFERSPROC GenBuffers;
        PFNGLGENVERTEXARRAYSPROC GenVertexArrays;
        PFNGLBINDBUFFERPROC BindBuffer;
        PFNGLBINDVERTEXARRAYPROC BindVertexArray;
        PFNGLBUFFERDATAPROC BufferData;
        PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer;
        PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
        PFNGLBUFFERSUBDATAPROC BufferSubData;
        PFNGLDRAWARRAYSEXTPROC DrawArrays;
        PFNGLDISABLEVERTEXATTRIBARRAYPROC DisableVertexAttribArray;
        PFNGLDELETEBUFFERSPROC DeleteBuffers;
        PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;
};

}  // namespace tire

#endif
