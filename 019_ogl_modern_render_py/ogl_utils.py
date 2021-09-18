
from OpenGL.GL import *

g_VS = """
#version 330 core
in vec3 position;
in vec3 color;

out vec3 newColor;
void main()
{
    gl_Position = vec4(position, 1.0f);
    newColor = color;
}
"""

g_FS = """
#version 330
in vec3 newColor;

out vec4 outColor;
void main()
{
    outColor = vec4(newColor, 1.0f);
}
"""

def setupShaders():
 
    v = glCreateShader(GL_VERTEX_SHADER)
    f = glCreateShader(GL_FRAGMENT_SHADER)
 
    glShaderSource(v,  g_VS)
    glShaderSource(f, g_FS)
 
    glCompileShader(v)
    glCompileShader(f)
 
    printShaderInfoLog(v)
    printShaderInfoLog(f)
 
    p = glCreateProgram()
    glAttachShader(p,v)
    glAttachShader(p,f)
 
    glBindFragDataLocation(p, 0, "outputF")
    glLinkProgram(p)
    printProgramInfoLog(p)
 
    vertexLoc = glGetAttribLocation(p,"position")
    colorLoc = glGetAttribLocation(p, "color")
 
    projMatrixLoc = glGetUniformLocation(p, "projMatrix")
    viewMatrixLoc = glGetUniformLocation(p, "viewMatrix")
 
    return p

def printOglError() -> int:
    # Returns 1 if an OpenGL error occurred, 0 otherwise.
 
    err = glGetError()

    retCode = 0

    while (err != GL_NO_ERROR):
        if err==GL_INVALID_ENUM:
                error = "INVALID_ENUM" 
        if err==GL_INVALID_VALUE:
                error = "INVALID_VALUE"
        if err==GL_INVALID_OPERATION:
                error = "INVALID_OPERATION"
        if err==GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW"
        if err==GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW"
        if err==GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY"
        if err==GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION"
        print(error)
        retCode = 1
        err = glGetError()

    return retCode

def printShaderInfoLog(obj: GLuint) -> None:
    infologLength = glGetShaderiv(obj, GL_INFO_LOG_LENGTH)
 
    if (infologLength > 0):
        infoLog = glGetShaderInfoLog(obj, infologLength)
        print(infoLog)

def printProgramInfoLog(obj):
    infologLength = 0
    charsWritten  = 0
 
    infologLength = glGetProgramiv(obj, GL_INFO_LOG_LENGTH)
 
    if (infologLength > 0):
        infoLog = glGetProgramInfoLog(obj, infologLength)
        print(infoLog)
