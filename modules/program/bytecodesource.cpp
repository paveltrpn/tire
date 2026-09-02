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
            const auto stage = stageType( item );

            if ( stage == ShaderStageType::UNKNOWN ) {
                const auto msg = std::format( "Unknown shader stage for file: {}", item );
                throw std::runtime_error( msg );
            }

            auto file = std::ifstream{ item, std::ios::binary | std::ios::ate };

            if ( !file.is_open() ) {
                const auto msg = std::format( "Failed to open file: {}", item );
                std::println( "=== {} ", msg );
                //throw std::runtime_error( msg );
            }

            // Get file size.
            const auto size = file.tellg();
            if ( size <= 0 ) {
                const auto msg = std::format( "File is empty or invalid: {}", item );
                throw std::runtime_error( msg );
            }

            // Seek back to beginning.
            file.seekg( 0, std::ios::beg );

            // Allocate vector and read.
            std::vector<uint8_t> bytecode( static_cast<size_t>( size ) );
            if ( !file.read( reinterpret_cast<char*>( bytecode.data() ), size ) ) {
                throw std::runtime_error( "Failed to read file: " + item );
            }

            _sources.emplace_back( stage, bytecode );
        }
    };

    [[nodiscard]]
    auto sources() const -> const std::vector<std::pair<ShaderStageType, std::vector<uint8_t>>>& {
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
    std::vector<std::pair<ShaderStageType, std::vector<uint8_t>>> _sources{};
};

}  // namespace tire