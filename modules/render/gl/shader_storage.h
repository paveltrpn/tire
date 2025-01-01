
#pragma once

#include <map>
#include <filesystem>

#include <GL/gl.h>

#include "algebra/concepts.h"
#include "algebra/vector2.h"
#include "algebra/vector3.h"
#include "algebra/vector4.h"
#include "algebra/matrix2.h"
#include "algebra/matrix3.h"
#include "algebra/matrix4.h"

#include "program.h"

namespace tire::gl {

struct ShaderStorage final {
    ShaderStorage() = default;

    ShaderStorage( const ShaderStorage &other ) = delete;
    ShaderStorage( ShaderStorage &&other ) = delete;
    ShaderStorage &operator=( const ShaderStorage &other ) = delete;
    ShaderStorage &operator=( ShaderStorage &&other ) = delete;

    ~ShaderStorage() = default;

    void add( const std::string &name, Program program );
    void use( const std::string &name );
    void destroy( const std::string &name );

    GLuint getUniformLocation( const std::string &name, const std::string &id );

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
    // "program name (string id)" - "shader program id"
    std::map<std::string, Program> programs_;
};

}  // namespace tire::gl
