
#ifndef __shader_h__
#define __shader_h__

#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include "render/GLFunctions.h"

namespace tire {

struct Shader final {
        Shader() = default;
        ~Shader();

        [[nodiscard]]
        GLuint getProgramId() const {
            return program_;
        };

        void linkProgram(std::vector<std::pair<GLuint, std::string>> shaders);
        void use();

        void showActiveUniforms();
        void showActiveAttributes();

        GLuint getUniform(const std::string &id);

    private:
        std::vector<GLuint> getShadersList(std::vector<std::pair<GLuint, std::string>> shaders);
        void getActiveAttributes();
        void getActiveUniforms();

    private:
        GLuint program_{};

        std::vector<std::pair<std::string, GLenum>> uniforms_;
        std::vector<std::pair<std::string, GLenum>> attributes_;
};

}  // namespace tire

#endif
