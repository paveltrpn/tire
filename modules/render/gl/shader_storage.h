
#pragma once

#include <map>
#include <filesystem>
#include <variant>

#include <GL/gl.h>

#include "program.h"

namespace tire::gl {

using ProgramType =
    std::variant<Program<ProgramTagType::SIMPLE>, Program<ProgramTagType::TWO>,
                 Program<ProgramTagType::THREE>>;

struct ShaderStorage final {
    ShaderStorage() = default;

    ShaderStorage( const ShaderStorage &other ) = delete;
    ShaderStorage( ShaderStorage &&other ) = delete;
    ShaderStorage &operator=( const ShaderStorage &other ) = delete;
    ShaderStorage &operator=( ShaderStorage &&other ) = delete;

    ~ShaderStorage() = default;

    void add( const std::string &name, const ProgramType &program );
    void use( const std::string &name );
    void destroy( const std::string &name );

private:
    // "program name (string id)" - "shader program id"
    std::map<std::string, ProgramType> programs_;
};

}  // namespace tire::gl
