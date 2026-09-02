module;

#include <string>
#include <vector>
#include <filesystem>

#include "log/log.h"

export module program : textsource;

import : programsource;

namespace tire {

export struct TextProgramSource final : ProgramSource {
public:
    TextProgramSource( std::string programName )
        : ProgramSource{ programName } {};

private:
    auto listDirectory( std::filesystem::path path ) -> std::vector<std::string> override {
        std::vector<std::string> results{};
        return results;
    }

private:
};

}  // namespace tire