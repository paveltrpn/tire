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

        const auto glslShadersList = listDirectory( spirvPath );
    };

    auto sources() const -> const std::unordered_map<ShaderStageType, std::string>& {
        //
        return _sources;
    }

private:
    auto listDirectory( std::filesystem::path path ) -> std::vector<std::string> override {
        std::vector<std::string> results{};

        for ( const auto& entry : std::filesystem::directory_iterator( path ) ) {
            if ( !entry.is_regular_file() ) {
                continue;
            }

            auto ext = entry.path().extension().string();

            // Convert to lowercase for comparison.
            std::transform( ext.begin(), ext.end(), ext.begin(), []( auto c ) -> decltype( c ) {
                //
                return std::tolower( c );
            } );

            if ( ext == ".glsl" ) {
                results.push_back( entry.path().string() );
            }
        }

        return results;
    }

private:
    std::unordered_map<ShaderStageType, std::string> _sources{};
};

}  // namespace tire