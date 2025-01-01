
#pragma once

#include <map>
#include <GL/gl.h>

namespace tire::gl {

struct ShaderStorage final {
    ShaderStorage() = default;

    ShaderStorage( const ShaderStorage &other ) = delete;
    ShaderStorage( ShaderStorage &&other ) = delete;
    ShaderStorage &operator=( const ShaderStorage &other ) = delete;
    ShaderStorage &operator=( ShaderStorage &&other ) = delete;

    ~ShaderStorage();

    void add();
    void use( const std::string &name );
    void destroy( const std::string &name );

private:
    // "program name (string id)" - "shader program id"
    std::map<std::string, GLuint> programs_;
};

}  // namespace tire::gl
