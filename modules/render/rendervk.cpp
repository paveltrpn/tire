
#include "rendervk.h"

import graph;

namespace tire {

RenderVK::RenderVK() {
    try {
        _ui = std::make_shared<UiVK>();

        _testBox = std::make_shared<TestBox>();
        _testBox->setPosition( 0.0f, 0.0f, -2.5f );

        _run = true;
    } catch ( const std::runtime_error& e ) {
        log::fatal()( "error white creating render {}", e.what() );
    }
}

auto RenderVK::isRun() -> bool {
    //
    return _run;
}

auto RenderVK::displayRenderInfo() -> void {
    //

};

auto RenderVK::attachScene( std::shared_ptr<Scene> scene ) -> void {
    _scene = std::move( scene );
}

}  // namespace tire