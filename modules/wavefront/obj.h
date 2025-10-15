
#pragma once

#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <format>
#include <filesystem>

namespace tire {

struct Obj final {
public:
    Obj() = delete;

    Obj( const Obj& rhs ) = delete;
    Obj( Obj&& rhs ) = delete;
    auto operator=( const Obj& rhs ) -> Obj& = delete;
    auto operator=( const Obj&& rhs ) -> Obj& = delete;

    Obj( std::string_view path );

    ~Obj() = default;

    auto assertFilePath( std::string_view path ) -> void;

    auto collectObjectInfo() -> void;

    friend auto operator<<( std::ostream& os, const Obj& rhs ) -> std::ostream&;

private:
    std::ifstream file_;

    std::string objectName_{};
    std::string matlibFileName_{};
    size_t verteciesCount_{ 0 };
    size_t normalsCount_{ 0 };
    size_t texCoordsCount_{ 0 };
    size_t facesCount_{ 0 };
};

auto operator<<( std::ostream& os, const Obj& rhs ) -> std::ostream&;

}  // namespace tire