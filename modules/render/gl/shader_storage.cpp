
#include <fstream>
#include <vector>

#include "shader_storage.h"
#include "functions.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_SHADER_STORAGE_GL_CPP{ true };

namespace tire::gl {

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
