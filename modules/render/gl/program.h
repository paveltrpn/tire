
#pragma once

#include <filesystem>
#include <vector>
#include <GL/gl.h>

#include "algebra/concepts.h"
#include "algebra/vector2.h"
#include "algebra/vector3.h"
#include "algebra/vector4.h"
#include "algebra/matrix2.h"
#include "algebra/matrix3.h"
#include "algebra/matrix4.h"

namespace tire::gl {

enum class ProgramTagType { SIMPLE, TWO, THREE };

template <GLuint Opt>
concept ShaderStage = ( Opt == GL_VERTEX_SHADER ) ||
                      ( Opt == GL_FRAGMENT_SHADER ) ||
                      ( Opt == GL_COMPUTE_SHADER ) ||
                      ( Opt == GL_GEOMETRY_SHADER ) ||
                      ( Opt == GL_TESS_CONTROL_SHADER ) ||
                      ( Opt == GL_TESS_EVALUATION_SHADER );

struct ProgramBase {
    ProgramBase() = default;
    ~ProgramBase() = default;

    template <GLuint stageT>
    requires ShaderStage<stageT> void init(
        const std::filesystem::path &path ) {
        const auto &sourceString = readSource( path );
        const auto stage = compileStage( stageT, sourceString );
        stages_.push_back( stage );
    }

    void link();
    void use();
    void clear();

    GLuint program() { return program_; };

    GLuint getUniformLocation( const std::string &id );

    template <algebra::VectorFloat T>
    void setVectorUniform( GLint location, T value ) {
        if constexpr ( std::is_same_v<T, algebra::vector2f> ) {
            glUniform2fv( location, 1, value.data() );
        } else if constexpr ( std::is_same_v<T, algebra::vector3f> ) {
            glUniform3fv( location, 1, value.data() );
        } else if constexpr ( std::is_same_v<typename T::value_type, float> &&
                              T::size == 4 ) {
            glUniform4fv( location, 1, value.data() );
        } else if constexpr ( std::is_same_v<typename T::value_type, int> &&
                              T::size == 2 ) {
            glUniform2iv( location, 1, value.data() );
        } else if constexpr ( std::is_same_v<typename T::value_type, int> &&
                              T::size == 3 ) {
            glUniform3iv( location, 1, value.data() );
        } else if constexpr ( std::is_same_v<typename T::value_type, int> &&
                              T::size == 4 ) {
            glUniform4iv( location, 1, value.data() );
        } else if constexpr ( std::is_same_v<typename T::value_type,
                                             unsigned int> &&
                              T::size == 2 ) {
            glUniform2uiv( location, 1, value.data() );
        } else if constexpr ( std::is_same_v<typename T::value_type,
                                             unsigned int> &&
                              T::size == 3 ) {
            glUniform3uiv( location, 1, value.data() );
        } else if constexpr ( std::is_same_v<typename T::value_type,
                                             unsigned int> &&
                              T::size == 4 ) {
            glUniform4uiv( location, 1, value.data() );
        }
    }

    // only declaration now
    template <algebra::MatrixDouble T>
    void setMatrixUniform( GLuint location, GLboolean transpose, T value );

    template <algebra::MatrixFloat T>
    void setMatrixUniform( GLuint location, GLboolean transpose, T value ) {
        if constexpr ( std::is_same_v<T, algebra::matrix2f> ) {
            glUniformMatrix2fv( location, 1, transpose, value.data() );
        } else if constexpr ( std::is_same_v<T, algebra::matrix3f> ) {
            glUniformMatrix3fv( location, 1, transpose, value.data() );
        } else if constexpr ( std::is_same_v<T, algebra::matrix4f> ) {
            glUniformMatrix4fv( location, 1, transpose, value.data() );
        }
    }

private:
    std::string readSource( const std::filesystem::path &path );
    GLuint compileStage( GLenum stage, std::string_view source );

private:
    // program, linked against some shader stages
    GLuint program_{};
    std::vector<GLuint> stages_{};
};

template <ProgramTagType T>
struct Program : ProgramBase {};

template <>
struct Program<ProgramTagType::SIMPLE> : ProgramBase {};

template <>
struct Program<ProgramTagType::TWO> : ProgramBase {};

template <>
struct Program<ProgramTagType::THREE> : ProgramBase {};

}  // namespace tire::gl
