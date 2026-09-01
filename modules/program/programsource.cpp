module;

#include <string>

export module program : programsource;

namespace tire {

export struct ProgramSource {
public:
    ProgramSource( std::string programName )
        : _programName{ std::move( programName ) } {};

private:
    std::string _programName{};
};

}  // namespace tire
