
#pragma once

#include <filesystem>
#include <vector>
#include <GL/gl.h>
#include <type_traits>

#include "functions.h"

#include "algebra/concepts.h"
#include "algebra/vector2.h"
#include "algebra/vector3.h"
#include "algebra/vector4.h"
#include "algebra/matrix2.h"
#include "algebra/matrix3.h"
#include "algebra/matrix4.h"

#include "image/color.h"
namespace tire::gl {

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

    virtual void findUniforms() = 0;

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

protected:
    // program, linked against some shader stages
    GLuint program_{};

private:
    std::vector<GLuint> stages_{};
};

// Shader program type tags, describes few shader program
// possible "kinds".
struct ProgramColorTag {};
struct ProgramTextureTag {};
struct ProgramFlatShadeTag {};

template <typename T>
concept ProgramTagType = std::is_same_v<T, ProgramColorTag> ||
                         std::is_same_v<T, ProgramTextureTag> ||
                         std::is_same_v<T, ProgramFlatShadeTag>;

template <typename T>
concept ProgramBaseChild =
    std::is_base_of_v<T, ProgramBase> &&requires( T child ) {
    typename T::type_tag;
};

template <ProgramTagType T>
struct Program : ProgramBase {};

template <>
struct Program<ProgramColorTag> : ProgramBase {
    using type_tag = ProgramColorTag;

    void findUniforms() override {
        glUseProgram( program_ );
        viewMatrixLocation_ = getUniformLocation( "view_matrix" );
        colorLocation_ = getUniformLocation( "color" );
        glUseProgram( 0 );
    };

    void setViewMatrix( const algebra::matrix4f &matrix ) {
        setMatrixUniform( viewMatrixLocation_, GL_FALSE, matrix );
    };

    void setColor( Colorf color ) {
        setVectorUniform(
            colorLocation_,
            algebra::vector3f{ color.r(), color.g(), color.b() } );
    };

private:
    GLuint viewMatrixLocation_{};
    GLuint colorLocation_{};
};

template <>
struct Program<ProgramTextureTag> : ProgramBase {
    using type_tag = ProgramColorTag;

    void findUniforms() override {
        viewMatrixLocation_ = getUniformLocation( "view_matrix" );
    };

private:
    GLuint viewMatrixLocation_{};
};

template <>
struct Program<ProgramFlatShadeTag> : ProgramBase {
    using type_tag = ProgramFlatShadeTag;

    void findUniforms() override {
        viewMatrixLocation_ = getUniformLocation( "view_matrix" );
    };

private:
    GLuint viewMatrixLocation_{};
};

}  // namespace tire::gl
