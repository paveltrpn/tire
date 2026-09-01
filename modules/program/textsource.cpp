module;

#include <string>
#include <vector>

#include "log/log.h"

export module program : textsource;

import : programsource;

namespace tire {

export struct TextProgramSource final : ProgramSource {
public:
    TextProgramSource( std::string programName )
        : ProgramSource{ programName } {};

private:
};

}  // namespace tire