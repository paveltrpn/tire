module;

export module graph : drawable;

import : leaf;

namespace tire {

// NOLINTNEXTLINE(cppcoreguidelines-virtual-class-destructor)
export struct Drawable : public Leaf {
    Drawable( const Drawable& other ) = delete;
    Drawable( Drawable&& other ) = delete;

    auto operator=( const Drawable& other ) -> Drawable& = delete;
    auto operator=( Drawable&& other ) -> Drawable& = delete;

private:
    ~Drawable() override = default;
};

}  // namespace tire
