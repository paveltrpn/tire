module;

#include <string>
#include <vector>
#include <filesystem>

#include "log/log.h"
#include "config/config.h"

export module program : textsource;

import : definitions;
import : programsource;

namespace tire {

export struct TextProgramSource final : ProgramSource {
public:
    TextProgramSource( std::string programName )
        : ProgramSource{ programName } {
        const auto basePath = Config::instance().basePath();
        const auto spirvPath = basePath / "shaders";

        const auto glslShadersList = listDirectory( spirvPath, programName, ".glsl" );
    };

    auto sources() const -> const std::unordered_map<ShaderStageType, std::string>& {
        //
        return _sources;
    }

private:
    std::unordered_map<ShaderStageType, std::string> _sources{};
};

}  // namespace tire