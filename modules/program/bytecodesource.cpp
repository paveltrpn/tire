module;

#include <print>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <fstream>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#include "log/log.h"
#include "config/config.h"

export module program : bytecodesource;

import : definitions;
import : programsource;

namespace tire {

export struct BytecodeProgramSource final : ProgramSource {
public:
    BytecodeProgramSource( std::string programName )
        : ProgramSource{ programName } {
        const auto basePath = Config::instance().basePath();
        const auto spirvPath = basePath / "shaders" / "spirv";

        const auto spvShadersList = listDirectory( spirvPath );

        for ( auto&& item : spvShadersList ) {
            auto file = std::ifstream{ item, std::ios::binary | std::ios::ate };

            if ( !file.is_open() ) {
                const auto msg = std::format( "Failed to open file: {}", item );
                throw std::runtime_error( msg );
            }

            // Get file size.
            const auto size = file.tellg();
            if ( size <= 0 ) {
                const auto msg = std::format( "File is empty or invalid: {}", item );
                throw std::runtime_error( msg );
            }

            // Seek back to beginning.
            file.seekg( 0, std::ios::beg );

            // Allocate vector and read
            std::vector<uint8_t> buffer( static_cast<size_t>( size ) );
            if ( !file.read( reinterpret_cast<char*>( buffer.data() ), size ) ) {
                throw std::runtime_error( "Failed to read file: " + item );
            }

            std::println( "{}", item );
        }
    };

    auto sources() const -> const std::unordered_map<ShaderStageType, std::vector<uint8_t>>& {
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

            if ( ext == ".spv" ) {
                results.push_back( entry.path().string() );
            }
        }

        return results;
    }

private:
    std::unordered_map<ShaderStageType, std::vector<uint8_t>> _sources{};
};

}  // namespace tire