
module;

#include <GL/gl.h>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <type_traits>
#include "spdlog/spdlog.h"

export module opengl_types:shader;

import toy_std;
import :glfunctions;

namespace tire::opengl {

template <typename T>
concept ConceptGLScalar = std::is_same_v<T, GLint> || std::is_same_v<T, GLuint>
                          || std::is_same_v<T, GLfloat> || std::is_same_v<T, GLdouble>;

export {
    enum class ShaderID {
        BASIC_COLOR,
        BASIC_TEXTURE,
    };

    struct Shader final {
            Shader() = default;
            ~Shader() = default;

            [[nodiscard]]
            GLuint getProgramId() const {
                return program_;
            };

            void link(std::vector<std::pair<GLuint, std::string>> shaders) {
                auto shaderList = getShadersList(shaders);

                program_ = glCreateProgram();

                for (const auto &shHandle : shaderList) {
                    glAttachShader(program_, shHandle);
                }

                glLinkProgram(program_);

                GLint success;
                glGetProgramiv(program_, GL_LINK_STATUS, &success);

                if (success == GL_FALSE) {
                    int32_t logLength;
                    glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &logLength);

                    std::vector<GLchar> log;
                    log.reserve(logLength);

                    glGetProgramInfoLog(program_, logLength, nullptr, log.data());

                    spdlog::error("can't link program with trace:\n{}", log.data());
                    return;
                }
            }

            void use() {
                glUseProgram(program_);
            }

            void reset() {
                glDeleteProgram(program_);
            }

            void showActiveUniforms() {
                for (const auto &[uniName, uniType] : uniforms_) {
                    std::print("Uniform name - {}, type - {}\n", uniName, uniType);
                }
            }

            void showActiveAttributes() {
                for (const auto &[attrName, attrType] : attributes_) {
                    std::print("Attribute name - {}, type - {}\n", attrName, attrType);
                }
            }

            GLuint getUniformLocation(const std::string &id) {
                return glGetUniformLocation(program_, id.c_str());
            }

            template <typename T>
            void setScalarUniform(GLint location, T value) {
            }

            template <toy::ConceptVector T>
            void setVectorUniform(GLint location, T value) {
                if constexpr (std::is_same_v<T, toy::algebra::vector2f>) {
                    glUniform2fv(location, 1, value.data());
                } else if constexpr (std::is_same_v<T, toy::algebra::vector3f>) {
                    glUniform3fv(location, 1, value.data());
                } else if constexpr (std::is_same_v<typename T::value_type, float>
                                     && T::size == 4) {
                    glUniform4fv(location, 1, value.data());
                } else if constexpr (std::is_same_v<typename T::value_type, int> && T::size == 2) {
                    glUniform2iv(location, 1, value.data());
                } else if constexpr (std::is_same_v<typename T::value_type, int> && T::size == 3) {
                    glUniform3iv(location, 1, value.data());
                } else if constexpr (std::is_same_v<typename T::value_type, int> && T::size == 4) {
                    glUniform4iv(location, 1, value.data());
                } else if constexpr (std::is_same_v<typename T::value_type, unsigned int>
                                     && T::size == 2) {
                    glUniform2uiv(location, 1, value.data());
                } else if constexpr (std::is_same_v<typename T::value_type, unsigned int>
                                     && T::size == 3) {
                    glUniform3uiv(location, 1, value.data());
                } else if constexpr (std::is_same_v<typename T::value_type, unsigned int>
                                     && T::size == 4) {
                    glUniform4uiv(location, 1, value.data());
                }
            }

            template <toy::ConceptMatrixFloat T>
            void setMatrixUniform(GLuint location, GLboolean transpose, T value) {
                if constexpr (std::is_same_v<T, toy::algebra::matrix2f>) {
                    glUniformMatrix2fv(location, 1, transpose, value.data());
                } else if constexpr (std::is_same_v<T, toy::algebra::matrix3f>) {
                    glUniformMatrix3fv(location, 1, transpose, value.data());
                } else if constexpr (std::is_same_v<T, toy::algebra::matrix4f>) {
                    glUniformMatrix4fv(location, 1, transpose, value.data());
                }
            }

        private:
            [[nodiscard]]
            std::vector<GLuint> getShadersList(
              std::vector<std::pair<GLuint, std::string>> shaders) {
                std::vector<GLuint> rt;
                if (shaders.size() < 2) {
                    spdlog::error(" wrong shader count!");
                    return rt;
                } else {
                    for (const auto &[type, source] : shaders) {
                        GLuint shHandle = glCreateShader(type);

                        const char *c_str = source.c_str();

                        glShaderSource(shHandle, 1, &c_str, nullptr);
                        glCompileShader(shHandle);

                        GLint success;
                        glGetShaderiv(shHandle, GL_COMPILE_STATUS, &success);

                        if (success == GL_FALSE) {
                            int32_t logLength;
                            glGetShaderiv(shHandle, GL_INFO_LOG_LENGTH, &logLength);

                            std::vector<GLchar> log;
                            log.reserve(logLength);
                            glGetShaderInfoLog(shHandle, logLength, nullptr, log.data());

                            spdlog::error("can't compile shader with trace:\n{}", log.data());
                            return rt;
                        }

                        rt.push_back(shHandle);
                    }
                    return rt;
                }
            }

            void getActiveAttributes() {
                GLint size;   // size of the variable
                GLenum type;  // type of the variable (float, vec3 or mat4, etc)

                constexpr GLsizei bufSize = 16;    // maximum name length
                std::array<GLchar, bufSize> name;  // variable name in GLSL
                GLsizei length;                    // name length

                GLint count;
                glGetProgramiv(program_, GL_ACTIVE_ATTRIBUTES, &count);

                for (GLint i = 0; i < count; i++) {
                    glGetActiveAttrib(
                      program_, (GLuint)i, bufSize, &length, &size, &type, name.data());
                    attributes_.push_back({ name.data(), type });
                }
            }

            void getActiveUniforms() {
                GLint size;   // size of the variable
                GLenum type;  // type of the variable (float, vec3 or mat4, etc)

                constexpr GLsizei bufSize = 16;    // maximum name length
                std::array<GLchar, bufSize> name;  // variable name in GLSL
                GLsizei length;                    // name length

                GLint count;
                glGetProgramiv(program_, GL_ACTIVE_UNIFORMS, &count);

                for (GLint i = 0; i < count; i++) {
                    glGetActiveUniform(
                      program_, (GLuint)i, bufSize, &length, &size, &type, name.data());
                    uniforms_.push_back({ name.data(), type });
                }
            }

        private:
            GLuint program_{};

            std::vector<std::pair<std::string, GLenum>> uniforms_;
            std::vector<std::pair<std::string, GLenum>> attributes_;
    };
}
}  // namespace tire::opengl
