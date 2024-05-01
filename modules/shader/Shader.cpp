
#include "Shader.h"
#include "spdlog/spdlog.h"

namespace tire {

void Shader::linkProgram(std::vector<std::pair<GLuint, std::string>> shaders) {
    auto shaderList = getShadersList(shaders);

    program_ = glCreateProgram();

    for (const auto &shHandle : shaderList) {
        gl->AttachShader(program_, shHandle);
    }

    gl->LinkProgram(program_);

    GLint success;
    gl->GetProgramiv(program_, GL_LINK_STATUS, &success);

    if (success == GL_FALSE) {
        int32_t logLength;
        gl->GetProgramiv(program_, GL_INFO_LOG_LENGTH, &logLength);

        std::shared_ptr<GLchar[]> log(new GLchar[logLength]);
        gl->GetProgramInfoLog(program_, logLength, nullptr, log.get());

        spdlog::error("can't link program with trace:\n{}", log.get());
        return;
    }
}

void Shader::use() {
    gl->UseProgram(program_);
}

std::vector<GLuint> Shader::getShadersList(std::vector<std::pair<GLuint, std::string>> shaders) {
    std::vector<GLuint> rt;
    if (shaders.size() < 2) {
        spdlog::error(" wrong shader count!");
        return rt;
    } else {
        for (const auto &[type, source] : shaders) {
            GLuint shHandle = gl->CreateShader(type);

            const char *c_str = source.c_str();

            gl->ShaderSource(shHandle, 1, &c_str, nullptr);
            gl->CompileShader(shHandle);

            GLint success;
            gl->GetShaderiv(shHandle, GL_COMPILE_STATUS, &success);

            if (success == GL_FALSE) {
                int32_t logLength;
                gl->GetShaderiv(shHandle, GL_INFO_LOG_LENGTH, &logLength);

                std::shared_ptr<GLchar[]> log(new GLchar[logLength]);
                gl->GetShaderInfoLog(shHandle, logLength, nullptr, log.get());

                spdlog::error("can't compile shader with trace:\n{}", log.get());
                return rt;
            }

            rt.push_back(shHandle);
        }
        return rt;
    }
}

void Shader::getActiveAttributes() {
    GLint size;   // size of the variable
    GLenum type;  // type of the variable (float, vec3 or mat4, etc)

    constexpr GLsizei bufSize = 16;  // maximum name length
    GLchar name[bufSize];            // variable name in GLSL
    GLsizei length;                  // name length

    GLint count;
    gl->GetProgramiv(program_, GL_ACTIVE_ATTRIBUTES, &count);

    for (GLint i = 0; i < count; i++) {
        gl->GetActiveAttrib(program_, (GLuint)i, bufSize, &length, &size, &type, name);
        attributes_.push_back({ name, type });
    }
}

void Shader::getActiveUniforms() {
    GLint size;   // size of the variable
    GLenum type;  // type of the variable (float, vec3 or mat4, etc)

    constexpr GLsizei bufSize = 16;  // maximum name length
    GLchar name[bufSize];            // variable name in GLSL
    GLsizei length;                  // name length

    GLint count;
    gl->GetProgramiv(program_, GL_ACTIVE_UNIFORMS, &count);

    for (GLint i = 0; i < count; i++) {
        gl->GetActiveUniform(program_, (GLuint)i, bufSize, &length, &size, &type, name);
        uniforms_.push_back({ name, type });
    }
}

void Shader::showActiveUniforms() {
    for (const auto &[uniName, uniType] : uniforms_) {
        std::print("Uniform name - {}, type - {}\n", uniName, uniType);
    }
}

void Shader::showActiveAttributes() {
    for (const auto &[attrName, attrType] : attributes_) {
        std::print("Attribute name - {}, type - {}\n", attrName, attrType);
    }
}

GLuint Shader::getUniform(const std::string &id) {
    return gl->GetUniformLocation(program_, id.c_str());
}

Shader::~Shader() {
    // for (auto &shader : shaderList) {
    // glDeleteShader(shader);
    // }

    gl->DeleteProgram(program_);
}

}  // namespace tire
