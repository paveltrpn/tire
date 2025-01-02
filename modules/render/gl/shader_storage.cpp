
#include <fstream>
#include <vector>

#include "shader_storage.h"
#include "functions.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_SHADER_STORAGE_GL_CPP{ true };

namespace tire::gl {

void ShaderStorage::add( const std::string &name, const ProgramType &program ) {
    programs_[name] = program;
}

void ShaderStorage::use( const std::string &name ) {
    std::visit(
        []( auto &&arg ) {
            using T = std::decay_t<decltype( arg )>;
            if constexpr ( std::is_same_v<typename T::type_tag,
                                          ProgramColorTag> ) {
            } else if constexpr ( requires( T a ) { a.use(); } )
                arg.use();
        },
        programs_[name] );
}

void ShaderStorage::destroy( const std::string &name ) {
    ProgramType program;
    try {
        program = programs_.at( name );
    } catch ( std::out_of_range &e ) {
        log::warning( "shader program {} not exist!", name );
        return;
    }
    std::visit( []( auto &&arg ) { arg.clear(); }, programs_[name] );
    programs_.erase( name );
}

}  // namespace tire::gl
