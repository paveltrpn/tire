
#pragma once

#include <filesystem>

#include <GL/gl.h>

namespace tire::gl {

struct Program final {
    Program() = default;
    ~Program() = default;

    void initVertexStage( const std::filesystem::path &path );
    void initTesselationControlStage( const std::filesystem::path &path );
    void initTesselationEvaluationStage( const std::filesystem::path &path );
    void initGeometryStage( const std::filesystem::path &path );
    void initComputeStage( const std::filesystem::path &path );
    void initFragmentStage( const std::filesystem::path &path );

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

    // shader stages
    GLuint vertex_{};
    GLuint tesselationControl_{};
    GLuint tesselationEvaluation_{};
    GLuint geometry_{};
    GLuint fragment_{};
    GLuint compute_{};
};

}  // namespace tire::gl
