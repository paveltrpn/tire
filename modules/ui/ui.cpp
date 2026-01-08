
module;

#include <variant>
#include <vector>

export module ui:ui;

import log;

import :billboard;
import :label;

namespace tire {

using UiComponent = std::variant<tire::Label, tire::Billboard>;

export struct Ui {
    Ui() = default;

    Ui( const Ui &other ) = delete;
    Ui( Ui &&other ) = delete;

    auto operator=( const Ui &other ) -> Ui & = delete;
    auto operator=( Ui &&other ) -> Ui & = delete;

    ~Ui() = default;

    auto label( int px, int py, const std::string &msg ) -> void {
        //
    }

    auto billboard( int px, int py, int sx, int sy ) -> void {
        //
    }

private:
    std::vector<UiComponent> componentsList_{};
};

}  // namespace tire