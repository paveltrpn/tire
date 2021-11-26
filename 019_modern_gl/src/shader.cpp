
#include <fstream>
#include <sstream>
#include <memory>
#include <tuple>

#include "shader.h"

void loadShader(std::string fname) {
    std::string shader;
    std::ifstream File;
    File.open(fname, std::ios::in);

    if (File) {
        shader.assign(std::istreambuf_iterator<char>(File), std::istreambuf_iterator<char>());
    }

    std::cout << shader << "\n";
}

void COglProgram::appendShader(std::vector<std::tuple<GLuint, std::string>> shaders) {
    if (shaders.size() < 2) {
        std::cout << fmt::format("COglProgram::appendShader(): ERROR! Wrong shader count!");
        std::exit(1);
    } else {
        for (const auto &[type, fname]: shaders) {
            // Читаем файл в одну строку за раз
            std::ifstream inFile;
            inFile.open(fname, std::ios::in); 

            if (!inFile) {
                std::cout << fmt::format("COglProgram::appendShader(): ERROR! Can't find file {}!", fname);
                std::exit(1);
            }
            
            std::stringstream strStream;
            strStream << inFile.rdbuf(); 
            // Закончили читать

            GLuint shHandle = glCreateShader(type);

            auto srcString = strStream.str();
            const char *c_str = srcString.c_str();

            glShaderSource(shHandle, 1, &c_str, nullptr);
            glCompileShader(shHandle);

            GLint success;
			glGetShaderiv(shHandle, GL_COMPILE_STATUS, &success);

            if (success == GL_FALSE) {
				int32_t logLength;
				glGetShaderiv(shHandle, GL_INFO_LOG_LENGTH, &logLength);

                std::shared_ptr<GLchar[]> log(new GLchar[logLength]);
				glGetShaderInfoLog(shHandle, logLength, nullptr, log.get());

				std::cout << fmt::format("COglProgram::appendShader(): ERROR! Can't compile shader - {} with trace:\n{}", fname, log.get());
                inFile.close();
                std::exit(1);
			}

            shaderList.push_back(shHandle);

            inFile.close();
        } 
    }
}

void COglProgram::linkProgram() {
    pHandle = glCreateProgram();

    for (const auto &shHandle: shaderList) {
        glAttachShader(pHandle, shHandle);
    }

    glLinkProgram(pHandle);

    GLint success;
	glGetProgramiv(pHandle, GL_LINK_STATUS, &success);

    if (success == GL_FALSE) {
		int32_t logLength;
		glGetProgramiv(pHandle, GL_INFO_LOG_LENGTH, &logLength);

        std::shared_ptr<GLchar[]> log(new GLchar[logLength]);
		glGetProgramInfoLog(pHandle, logLength, nullptr, log.get());

		std::cout << fmt::format("COglProgram::linkProgram(): ERROR! Can't link program with trace:\n{}", log.get());
        std::exit(1);
	}

    getActiveUniforms();
    getActiveAttributes();
}

void COglProgram::use() {
    glUseProgram(pHandle);
};

void COglProgram::getActiveAttributes() {
    GLint i;
    GLint count;

    GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)

    const GLsizei bufSize = 16; // maximum name length
    GLchar name[bufSize]; // variable name in GLSL
    GLsizei length; // name length

    glGetProgramiv(pHandle, GL_ACTIVE_ATTRIBUTES, &count);

    for (i = 0; i < count; i++) {
        glGetActiveAttrib(pHandle, (GLuint)i, bufSize, &length, &size, &type, name);
        pAttributes.push_back({name, type});
    }
}

void COglProgram::getActiveUniforms() {
    GLint i;
    GLint count;

    GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)

    const GLsizei bufSize = 16; // maximum name length
    GLchar name[bufSize]; // variable name in GLSL
    GLsizei length; // name length

    glGetProgramiv(pHandle, GL_ACTIVE_UNIFORMS, &count);

    for (i = 0; i < count; i++) {
        glGetActiveUniform(pHandle, (GLuint)i, bufSize, &length, &size, &type, name);
        pUniforms.push_back({name, type});
    }
}

void COglProgram::showActiveUniforms() {
    for (const auto &[uniName, uniType]: pUniforms) {
        std::cout << fmt::format("Uniform name - {}, type - {}\n", uniName, uniType);
    }
}

void COglProgram::showActiveAttributes() {
    for (const auto &[attrName, attrType]: pAttributes) {
        std::cout << fmt::format("Attribute name - {}, type - {}\n", attrName, attrType);
    }
}

COglProgram::~COglProgram() {
    for (auto &shader: shaderList) {
        glDeleteShader(shader);
    }

    glDeleteProgram(pHandle);
}
