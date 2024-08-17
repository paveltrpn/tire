
#ifndef __shader_h__
#define __shader_h__

#include <GL/gl.h>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <type_traits>

#include "GLFunctions.h"

import toy_std;

namespace tire::opengl {

template <typename T>
concept ConceptGLScalar = std::is_same_v<T, GLint> || std::is_same_v<T, GLuint>
                          || std::is_same_v<T, GLfloat> || std::is_same_v<T, GLdouble>;

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

        void link(std::vector<std::pair<GLuint, std::string>> shaders);
        void use();

        void reset();

        void showActiveUniforms();
        void showActiveAttributes();

        GLuint getUniformLocation(const std::string &id);

        template <typename T>
        void setScalarUniform(GLint location, T value) {
        }

        template <toy::ConceptVector T>
        void setVectorUniform(GLint location, T value) {
            if constexpr (std::is_same_v<typename T::scalar_type, float> && T::size == 2) {
                glUniform2fv(location, 1, value.data());
            } else if constexpr (std::is_same_v<typename T::scalar_type, float> && T::size == 3) {
                glUniform3fv(location, 1, value.data());
            } else if constexpr (std::is_same_v<typename T::scalar_type, float> && T::size == 4) {
                glUniform4fv(location, 1, value.data());
            } else if constexpr (std::is_same_v<typename T::scalar_type, int> && T::size == 2) {
                glUniform2iv(location, 1, value.data());
            } else if constexpr (std::is_same_v<typename T::scalar_type, int> && T::size == 3) {
                glUniform3iv(location, 1, value.data());
            } else if constexpr (std::is_same_v<typename T::scalar_type, int> && T::size == 4) {
                glUniform4iv(location, 1, value.data());
            } else if constexpr (std::is_same_v<typename T::scalar_type, unsigned int>
                                 && T::size == 2) {
                glUniform2uiv(location, 1, value.data());
            } else if constexpr (std::is_same_v<typename T::scalar_type, unsigned int>
                                 && T::size == 3) {
                glUniform3uiv(location, 1, value.data());
            } else if constexpr (std::is_same_v<typename T::scalar_type, unsigned int>
                                 && T::size == 4) {
                glUniform4uiv(location, 1, value.data());
            }
        }

        template <tire::algebra::ConceptMatrixFloat T>
        void setMatrixUniform(GLuint location, GLboolean transpose, T value) {
            if constexpr (std::is_same_v<typename T::scalar_type, float> && T::size == 2) {
                glUniformMatrix2fv(location, 1, transpose, value.data());
            } else if constexpr (std::is_same_v<typename T::scalar_type, float> && T::size == 3) {
                glUniformMatrix3fv(location, 1, transpose, value.data());
            } else if constexpr (std::is_same_v<typename T::scalar_type, float> && T::size == 4) {
                glUniformMatrix4fv(location, 1, transpose, value.data());
            }
        }

    private:
        [[nodiscard]]
        std::vector<GLuint> getShadersList(std::vector<std::pair<GLuint, std::string>> shaders);
        void getActiveAttributes();
        void getActiveUniforms();

    private:
        GLuint program_{};

        std::vector<std::pair<std::string, GLenum>> uniforms_;
        std::vector<std::pair<std::string, GLenum>> attributes_;
};

}  // namespace tire::opengl

#endif
