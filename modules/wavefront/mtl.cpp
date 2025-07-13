module;

#include <string>
#include <format>

export module wavefront:mtl;

namespace tire {

export struct Mtl final {
    Mtl() = delete;

    Mtl( const Mtl &rhs ) = delete;
    Mtl( Mtl &&rhs ) = delete;
    Mtl &operator=( const Mtl &rhs ) = delete;
    Mtl &operator=( const Mtl &&rhs ) = delete;

    Mtl( std::string_view path ) {}

    ~Mtl() = default;

private:
};

}  // namespace tire
