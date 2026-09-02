module;

#include <print>
#include <string>
#include <filesystem>
#include <vector>

#include "log/log.h"

export module program : programsource;

import : definitions;

namespace tire {

export struct ProgramSource {
public:
    ProgramSource() = delete;

    ProgramSource( std::string programName )
        : _programName{ std::move( programName ) } {
            //
        };

    ProgramSource( const ProgramSource& other ) = default;
    ProgramSource( ProgramSource&& other ) = default;

    auto operator=( const ProgramSource& other ) -> ProgramSource& = default;
    auto operator=( ProgramSource&& other ) -> ProgramSource& = default;

    virtual ~ProgramSource() = default;

protected:
    virtual auto listDirectory( std::filesystem::path path ) -> std::vector<std::string> = 0;

    auto stageType( const std::string& shaderFileName ) -> ShaderStageType {
        const auto path = std::filesystem::path{ shaderFileName };
        const auto filenameWithoutExt = path.stem().string();

        if ( !isValidName( filenameWithoutExt ) ) {
            const auto msg = std::format( "Shader file invalid name: {}", filenameWithoutExt );
            log::error()( "{}", msg );
            return ShaderStageType::UNKNOWN;
        }

        // Split given string by seperator
        auto split = []( const std::string& string, const char* sep ) -> std::vector<std::string> {
            std::vector<std::string> list;
            std::string::size_type start{};
            std::string::size_type end{};

            while ( ( end = string.find( sep, start ) ) != std::string::npos ) {
                if ( start != end ) list.push_back( string.substr( start, end - start ) );
                start = end + 1;
            }

            if ( start != string.size() ) {
                list.push_back( string.substr( start ) );
            }

            return list;
        };

        const auto shaderNameParts = split( filenameWithoutExt, "_" );
        if ( shaderNameParts.size() < 3 ) {
            const auto msg = std::format( "Something wrong with shader name: {}", filenameWithoutExt );
            log::error()( "{}", msg );
            return ShaderStageType::UNKNOWN;
        }

        const auto& suffix = shaderNameParts.back();

        std::println( " === {}", suffix );

        try {
            return ShaderStageMap.at( suffix );
        } catch ( const std::out_of_range& e ) {
            return ShaderStageType::UNKNOWN;
        }
    }

private:
    // Finds out that given shader file name contains somthing from
    // shader stage suffix set ("VERTEX", "FRAGMENT" etc.)
    auto isValidName( const std::string& name ) -> bool {
        const auto end = StagesSuffixMap.cend();
        const auto it = std::find_if( StagesSuffixMap.cbegin(), end,
                                      [name]( std::pair<ShaderStageType, std::string> item ) -> bool {
                                          const auto [key, value] = item;
                                          return name.ends_with( value );
                                      } );

        return it != end;
    }

private:
    std::string _programName{};
};

}  // namespace tire
