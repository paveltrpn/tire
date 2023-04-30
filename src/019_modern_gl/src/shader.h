
#ifndef __shader_h__
#define __shader_h__

#include <fmt/format.h>
#include <iostream>
#include <string>
#include <vector>

#define GLEW_STATIC
#include <GL/gl.h>
#include <GL/glew.h>
#include <GL/glu.h>

class COglProgram {
    private:
        std::string pName;
        GLuint pHandle;
        std::vector<GLuint> shaderList;

        std::vector<std::tuple<std::string, GLenum>> pUniforms;
        std::vector<std::tuple<std::string, GLenum>> pAttributes;

    public:
        void appendShader(std::vector<std::tuple<GLuint, std::string>> shaders);
        void linkProgram();
        void use();

        void getActiveUniforms();
        void getActiveAttributes();

        void showActiveUniforms();
        void showActiveAttributes();

        ~COglProgram();
};

void loadShader(std::string fname);

#endif
