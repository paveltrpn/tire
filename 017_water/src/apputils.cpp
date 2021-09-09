
#include <GL/glew.h>

#include "apputils.h"

void CAppState::showCurrentAppState() {
    std::cout << fmt::format("CAppState::showCurrentAppState(): current application state is:\n");

    std::cout << fmt::format("  appWindowWidth - {}\n  appWindowHeight - {}\n  appWindowAspect - {}\n  appName - {}\n", 
                            appWindowWidth, appWindowHeight, appWindowAspect, appName);

    std::cout << "\n";
}

GLenum checkOpenGLError(const std::source_location lctn) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " in file - " << lctn.file_name() << "; line - " << lctn.line() << std::endl;
    }
    return errorCode;
}