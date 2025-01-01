
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

}  // namespace tire::gl
