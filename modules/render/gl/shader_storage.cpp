
#include <fstream>
#include <vector>

#include "shader_storage.h"
#include "functions.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_SHADER_STORAGE_GL_CPP{ true };

namespace tire::gl {

Program::~Program() {
    glDeleteProgram( vertex_ );
    glDeleteProgram( tesselationControl_ );
    glDeleteProgram( tesselationEvaluation_ );
    glDeleteProgram( geometry_ );
    glDeleteProgram( fragment_ );
    glDeleteProgram( compute_ );
    glDeleteProgram( program_ );
}

void Program::link() {
    program_ = glCreateProgram();
    glAttachShader( program_, vertex_ );
    glAttachShader( program_, fragment_ );

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

void Program::initVertexStage( const std::filesystem::path &path ) {
    const auto &sourceString = readSource( path );
    vertex_ = compileStage( GL_VERTEX_SHADER, sourceString );
}

void Program::initTesselationControlStage( const std::filesystem::path &path ) {
    const auto &sourceString = readSource( path );
    tesselationControl_ = compileStage( GL_TESS_CONTROL_SHADER, sourceString );
}

void Program::initTesselationEvaluationStage(
    const std::filesystem::path &path ) {
    const auto &sourceString = readSource( path );
    tesselationEvaluation_ =
        compileStage( GL_TESS_EVALUATION_SHADER, sourceString );
}

void Program::initGeometryStage( const std::filesystem::path &path ) {
    const auto &sourceString = readSource( path );
    geometry_ = compileStage( GL_GEOMETRY_SHADER, sourceString );
}

void Program::initComputeStage( const std::filesystem::path &path ) {
    const auto &sourceString = readSource( path );
    compute_ = compileStage( GL_COMPUTE_SHADER, sourceString );
}

void Program::initFragmentStage( const std::filesystem::path &path ) {
    const auto &sourceString = readSource( path );
    fragment_ = compileStage( GL_FRAGMENT_SHADER, sourceString );
}

std::string Program::readSource( const std::filesystem::path &path ) {
    log::debug<DEBUG_OUTPUT_SHADER_STORAGE_GL_CPP>( "loading shader file {}",
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

// ============================================================================

void ShaderStorage::add( const std::string &name, Program program ) {
    programs_[name] = program;
}

void ShaderStorage::use( const std::string &name ) {
    programs_[name].use();
}

void ShaderStorage::destroy( const std::string &name ) {
    Program program;
    try {
        program = programs_.at( name );
    } catch ( std::out_of_range &e ) {
        log::warning( "shader program {} not exist!", name );
        return;
    }
    programs_.erase( name );
}

GLuint ShaderStorage::getUniformLocation( const std::string &name,
                                          const std::string &id ) {
    const auto program = programs_[name].program();
    const auto location = glGetUniformLocation( program, id.c_str() );

    if ( location == GL_INVALID_VALUE ) {
        log::warning( "uniform location error - invalid value" );
    }

    if ( location == GL_INVALID_OPERATION ) {
        log::warning( "uniform location error - invalid operation" );
    }

    return location;
}

}  // namespace tire::gl
