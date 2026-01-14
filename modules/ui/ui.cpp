
module;

#include <string>
#include <variant>
#include <forward_list>

export module ui:ui;

import log;

import :billboard;
import :label;

namespace tire {

using UiComponent = std::variant<tire::Label, tire::Billboard>;

export struct Ui {
    Ui() {
        // Preallocate.
        // componentsList_.resize( 32 );
    };

    Ui( const Ui &other ) = delete;
    Ui( Ui &&other ) = delete;

    auto operator=( const Ui &other ) -> Ui & = delete;
    auto operator=( Ui &&other ) -> Ui & = delete;

    virtual ~Ui() = default;

    auto label( float px, float py, const std::string &msg ) -> void {
        auto l = tire::Label{};

        l.setColor( { "navy" } );
        l.setGlyphGap( 0.5f );
        l.setLabelPos( px, py );
        l.draw( msg );

        componentsList_.emplace_front( std::move( l ) );
    }

    auto billboard( float px, float py, float sx, float sy ) -> void {
        //
        auto b = tire::Billboard{};

        componentsList_.emplace_front( std::move( b ) );
    }

    virtual auto flush() -> void = 0;

protected:
    std::forward_list<UiComponent> componentsList_{};
};

}  // namespace tire