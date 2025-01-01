
#pragma once

#include <map>
#include <filesystem>
#include <GL/gl.h>

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

private:
    // "program name (string id)" - "shader program id"
    std::map<std::string, Program> programs_;
};

}  // namespace tire::gl
