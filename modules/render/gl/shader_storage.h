
#pragma once

#include <map>
#include <filesystem>
#include <variant>
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

enum class ShaderStageType {
    VERTEX,
    FRAGMENT,
    TESSELATION_EVAL,
    TESSELATION_CTRL,
    COMPUTE,
    GEOMETRY,
    MESH
};

static constexpr std::string vertex_stage_suffix{ "VERTEX" };
static constexpr std::string tessctrl_stage_suffix{ "TESSCTRL" };
static constexpr std::string tesseval_stage_suffix{ "TESSEVAL" };
static constexpr std::string geometry_stage_suffix{ "GEOMETRY" };
static constexpr std::string fragment_stage_suffix{ "FRAGMENT" };
static constexpr std::string compute_stage_suffix{ "COMPUTE" };
static constexpr std::string mesh_stage_suffix{ "MESH" };

template <ShaderStageType Stage>
concept ShaderStageX =
    ( Stage == ShaderStageType::VERTEX ) ||
    ( Stage == ShaderStageType::FRAGMENT ) ||
    ( Stage == ShaderStageType::TESSELATION_EVAL ) ||
    ( Stage == ShaderStageType::TESSELATION_CTRL ) ||
    ( Stage == ShaderStageType::GEOMETRY ) ||
    ( Stage == ShaderStageType::COMPUTE ) || ( Stage == ShaderStageType::MESH );

static const std::map<std::string, GLenum> StagesSuffixMap = {
    { vertex_stage_suffix, GL_VERTEX_SHADER },
    { fragment_stage_suffix, GL_FRAGMENT_SHADER },
    { tesseval_stage_suffix, GL_TESS_EVALUATION_SHADER },
    { tessctrl_stage_suffix, GL_TESS_CONTROL_SHADER },
    { geometry_stage_suffix, GL_GEOMETRY_SHADER },
    { compute_stage_suffix, GL_COMPUTE_SHADER },
    // { mesh_stage_suffix, GL_MESH_SHADER },
};

struct ShaderStorage final {
    ShaderStorage() = default;

    ShaderStorage( const ShaderStorage &other ) = delete;
    ShaderStorage( ShaderStorage &&other ) = delete;
    ShaderStorage &operator=( const ShaderStorage &other ) = delete;
    ShaderStorage &operator=( ShaderStorage &&other ) = delete;

    ~ShaderStorage() = default;

    void add( const std::string &name );

    void use( const std::string &name );

    void destroy( const std::string &name );

    template <typename T>
        requires std::is_same_v<T, float> || std::is_same_v<T, double> ||
        std::is_same_v<T, unsigned int> ||
        std::is_same_v<T, int> void setScalarUniform( const std::string &name,
                                                      const char *id,
                                                      T value ) {
        const auto location = getUniformLocation( name, id );

        if constexpr ( std::is_same_v<T, float> ) {
            glUniform1f( location, value );
        } else if constexpr ( std::is_same_v<T, double> ) {
            glUniform1f( location, value );
        } else {
            glUniform1i( location, value );
        }
    }

    template <algebra::VectorFloat T>
    void setVectorUniform( const std::string &name, const char *id, T value ) {
        const auto location = getUniformLocation( name, id );

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
    void setMatrixUniform( const std::string &name, const char *id,
                           GLboolean transpose, T value ) {
        const auto location = getUniformLocation( name, id );

        if constexpr ( std::is_same_v<T, algebra::matrix2f> ) {
            glUniformMatrix2fv( location, 1, transpose, value.data() );
        } else if constexpr ( std::is_same_v<T, algebra::matrix3f> ) {
            glUniformMatrix3fv( location, 1, transpose, value.data() );
        } else if constexpr ( std::is_same_v<T, algebra::matrix4f> ) {
            glUniformMatrix4fv( location, 1, transpose, value.data() );
        }
    }

private:
    std::vector<std::pair<std::string, GLenum>> scanForShaderFiles(
        const std::string &name );
    std::string readSource( const std::string &name );
    GLuint compile( GLenum stage, std::string_view source );

    GLuint getUniformLocation( const std::string &name, const char *id );

private:
    // "program name (string id)" - "shader program"
    std::map<std::string, GLuint> programs_;
};

}  // namespace tire::gl
