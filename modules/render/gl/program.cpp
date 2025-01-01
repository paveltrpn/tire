
#include <fstream>
#include <vector>

#include "program.h"
#include "functions.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_SHADER_PROGRAM_GL_CPP{ true };

namespace tire::gl {

void Program::clear() {
    for ( const auto handle : stages_ ) {
        glDeleteProgram( handle );
    }
}

void Program::link() {
    program_ = glCreateProgram();

    for ( const auto handle : stages_ ) {
        glAttachShader( program_, handle );
    }

    glLinkProgram( program_ );

    GLint success;
    glGetProgramiv( program_, GL_LINK_STATUS, &success );

    if ( success == GL_FALSE ) {
        int32_t logLength;
        glGetProgramiv( program_, GL_INFO_LOG_LENGTH, &logLength );
        std::vector<GLchar> log;
        log.reserve( logLength );
        glGetProgramInfoLog( program_, logLength, nullptr, log.data() );
        log::error( "can't link program with trace:\n{}", log.data() );
    }
}

void Program::use() {
    glUseProgram( program_ );
}

std::string Program::readSource( const std::filesystem::path &path ) {
    log::debug<DEBUG_OUTPUT_SHADER_PROGRAM_GL_CPP>( "loading shader file {}",
                                                    path.filename().string() );
    std::ifstream file;
    file.exceptions( std::ifstream::badbit );
    try {
        file.open( path );
    } catch ( const std::ifstream::failure &e ) {
        throw std::runtime_error(
            std::format( "file not found: {}\n", path.filename().string() ) );
    }

    std::string str;

    file.seekg( 0, std::ios::end );
    str.reserve( file.tellg() );
    file.seekg( 0, std::ios::beg );

    str.assign( ( std::istreambuf_iterator<char>( file ) ),
                std::istreambuf_iterator<char>() );

    file.close();

    return str;
}

GLuint Program::compileStage( GLenum stage, std::string_view source ) {
    const GLuint shHandle = glCreateShader( stage );
    const char *c_str = source.data();

    glShaderSource( shHandle, 1, &c_str, nullptr );
    glCompileShader( shHandle );

    GLint success;

    glGetShaderiv( shHandle, GL_COMPILE_STATUS, &success );
    if ( success == GL_FALSE ) {
        int32_t logLength;
        glGetShaderiv( shHandle, GL_INFO_LOG_LENGTH, &logLength );
        std::vector<GLchar> log;
        log.reserve( logLength );
        glGetShaderInfoLog( shHandle, logLength, nullptr, log.data() );
        log::error( "can't compile shader with trace:\n{}", log.data() );
        return 0;
    } else {
        return shHandle;
    }
}
}  // namespace tire::gl
