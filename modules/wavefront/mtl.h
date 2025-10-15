
#include <string>
#include <format>

namespace tire {

struct Mtl final {
    Mtl() = delete;

    Mtl( const Mtl& rhs ) = delete;
    Mtl( Mtl&& rhs ) = delete;
    auto operator=( const Mtl& rhs ) -> Mtl& = delete;
    auto operator=( const Mtl&& rhs ) -> Mtl& = delete;

    Mtl( std::string_view path ) {}

    ~Mtl() = default;

private:
};

}  // namespace tire
