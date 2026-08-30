module;

export module graph : drawable;

import : leaf;
import : visitor;

namespace tire {

// NOLINTNEXTLINE(cppcoreguidelines-virtual-class-destructor)
export struct Drawable : public Leaf {
    Drawable( const Drawable& other ) = delete;
    Drawable( Drawable&& other ) = delete;

    auto operator=( const Drawable& other ) -> Drawable& = delete;
    auto operator=( Drawable&& other ) -> Drawable& = delete;

    auto accept( Visitor& visitor ) -> void override{
        //
    }

private:
    ~Drawable() override = default;
};

}  // namespace tire
