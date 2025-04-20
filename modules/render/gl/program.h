
#pragma once

#include <map>
#include <filesystem>
#include <unordered_map>
#include <variant>
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

#include "log/log.h"

namespace tire::gl {

using namespace algebra;

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

struct Program final {
    Program() = default;
    ~Program() { clean(); };

    void init( const std::string &name );
    void addUniform( const std::string &id );
    void use();
    void clean();

    template <typename T>
        requires std::is_same_v<T, float> || std::is_same_v<T, double> ||
        std::is_same_v<T, unsigned int> ||
        std::is_same_v<T, int> void setScalarUniform( const std::string &id,
                                                      T value ) {
        if ( !uniforms_.contains( id ) ) {
            log::warning( "gl::Program === program {} not contains uniform {}",
                          name_, id );
            return;
        }

        const auto location = uniforms_[id];

        if constexpr ( std::is_same_v<T, float> ) {
            glUniform1f( location, value );
        } else if constexpr ( std::is_same_v<T, double> ) {
            // TODO: a way to pass double???
            glUniform1d( location, static_cast<float>( value ) );
        } else if constexpr ( std::is_same_v<T, int> ) {
            glUniform1i( location, value );
        } else if constexpr ( std::is_same_v<T, unsigned int> ) {
            glUniform1ui( location, value );
        }
    }

    template <algebra::VectorDouble T>
    void setVectorUniform( const std::string &id, T value ) {
        if ( !uniforms_.contains( id ) ) {
            log::warning( "gl::Program === program {} not contains uniform {}",
                          name_, id );
            return;
        }

        const auto location = uniforms_[id];

        if constexpr ( std::is_same_v<T, vector2d> ) {
            glUniform2dv( location, 1, value.data() );
        } else if constexpr ( std::is_same_v<T, vector3d> ) {
            glUniform3dv( location, 1, value.data() );
        } else if constexpr ( std::is_same_v<T, vector4d> ) {
            glUniform4dv( location, 1, value.data() );
        }
    }

    template <algebra::VectorFloat T>
    void setVectorUniform( const std::string &id, T value ) {
        if ( !uniforms_.contains( id ) ) {
            log::warning( "gl::Program === program {} not contains uniform {}",
                          name_, id );
            return;
        }

        const auto location = uniforms_[id];

        if constexpr ( std::is_same_v<T, algebra::vector2f> ) {
            glUniform2fv( location, 1, value.data() );
        } else if constexpr ( std::is_same_v<T, algebra::vector3f> ) {
            glUniform3fv( location, 1, value.data() );
        } else if constexpr ( std::is_same_v<T, vector4f> ) {
            glUniform4fv( location, 1, value.data() );
        }
    }

    template <algebra::VectorInt T>
    void setVectorUniform( const std::string &id, T value ) {
        if ( !uniforms_.contains( id ) ) {
            log::warning( "gl::Program === program {} not contains uniform {}",
                          name_, id );
            return;
        }

        const auto location = uniforms_[id];

        if constexpr ( std::is_same_v<T, algebra::vector2f> ) {
            glUniform2iv( location, 1, value.data() );
        } else if constexpr ( std::is_same_v<T, algebra::vector3f> ) {
            glUniform3iv( location, 1, value.data() );
        } else if constexpr ( std::is_same_v<T, vector4f> ) {
            glUniform4iv( location, 1, value.data() );
        }
    }

    template <algebra::VectorUInt T>
    void setVectorUniform( const std::string &id, T value ) {
        if ( !uniforms_.contains( id ) ) {
            log::warning( "gl::Program === program {} not contains uniform {}",
                          name_, id );
            return;
        }

        const auto location = uniforms_[id];

        if constexpr ( std::is_same_v<T, algebra::vector2f> ) {
            glUniform2uiv( location, 1, value.data() );
        } else if constexpr ( std::is_same_v<T, algebra::vector3f> ) {
            glUniform3uiv( location, 1, value.data() );
        } else if constexpr ( std::is_same_v<T, vector4f> ) {
            glUniform4uiv( location, 1, value.data() );
        }
    }

    template <algebra::MatrixDouble T>
    void setMatrixUniform( const std::string &id, GLboolean transpose,
                           T value ) {
        if ( !uniforms_.contains( id ) ) {
            log::warning( "gl::Program === program {} not contains uniform {}",
                          name_, id );
            return;
        }

        const auto location = uniforms_[id];

        if constexpr ( std::is_same_v<T, algebra::matrix2d> ) {
            glUniformMatrix2dv( location, 1, transpose, value.data() );
        } else if constexpr ( std::is_same_v<T, algebra::matrix3d> ) {
            glUniformMatrix3dv( location, 1, transpose, value.data() );
        } else if constexpr ( std::is_same_v<T, algebra::matrix4d> ) {
            glUniformMatrix4dv( location, 1, transpose, value.data() );
        }
    }

    template <algebra::MatrixFloat T>
    void setMatrixUniform( const std::string &id, GLboolean transpose,
                           T value ) {
        if ( !uniforms_.contains( id ) ) {
            log::warning( "gl::Program === program {} not contains uniform {}",
                          name_, id );
            return;
        }

        const auto location = uniforms_[id];

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

    GLuint getUniformLocation( const std::string &id );

private:
    GLuint program_{};
    std::string name_{};
    std::unordered_map<std::string, GLint> uniforms_{};
};

}  // namespace tire::gl
