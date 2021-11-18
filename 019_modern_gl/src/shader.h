
#ifndef __shader_h__
#define __shader_h__

#include <string>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>

class COglProgram {
    private:
        std::string     programName;
        GLuint          programId;
        std::vector<GLuint> shaderList;

        void appendShader(const std::vector<std::string> &types, const std::vector<std::string> &files);
        void linkProgram();
        void use();
};

#endif
