
#include "shader_storage.h"
#include "functions.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_SHADER_STORAGE_GL_CPP{ false };

namespace tire::gl {

ShaderStorage::~ShaderStorage() {
    for ( const auto program : programs_ ) {
        glDeleteProgram( std::get<1>( program ) );
    }
}

void ShaderStorage::add() {
}

void ShaderStorage::use( const std::string &name ) {
    glUseProgram( programs_[name] );
}

void ShaderStorage::destroy( const std::string &name ) {
    GLuint program;
    try {
        program = programs_.at( name );
    } catch ( std::out_of_range &e ) {
        log::warning( "shader program {} not exist!", name );
        return;
    }
    glDeleteProgram( program );
    programs_.erase( name );
}

}  // namespace tire::gl
