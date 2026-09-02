module;

#include <string>
#include <filesystem>
#include <vector>

export module program : programsource;

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

private:
    std::string _programName{};
};

}  // namespace tire
