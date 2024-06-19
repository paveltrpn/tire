
#ifndef __shadersources_h__
#define __shadersources_h__

#include <iostream>
#include <string>
#include <map>

namespace tire {

struct ShaderDatabase final {
        ShaderDatabase();
        std::string getVertexShader(const std::string &shaderId);
        std::string getFragmentShader(const std::string &shaderId);

    private:
        std::map<std::string, std::string> vertexShaderSources_;
        std::map<std::string, std::string> fragmentShaderSources_;
};

}  // namespace tire

#endif
