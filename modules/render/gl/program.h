
#pragma once

#include <filesystem>
#include <vector>

#include <GL/gl.h>

namespace tire::gl {

struct Program {
    Program() = default;
    ~Program() = default;

    template <GLuint stageT>
    void init( const std::filesystem::path &path ) {
        const auto &sourceString = readSource( path );
        const auto stage = compileStage( stageT, sourceString );
        stages_.push_back( stage );
    }

    void link();
    void use();
    void clear();

    GLuint program() { return program_; };

private:
    std::string readSource( const std::filesystem::path &path );
    GLuint compileStage( GLenum stage, std::string_view source );

private:
    // program, linked against some shader stages
    GLuint program_{};
    std::vector<GLuint> stages_{};
};

}  // namespace tire::gl
