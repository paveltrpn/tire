
#include "rendervk.h"
#include "log/log.h"

namespace tire {

RenderVK::RenderVK() {
    try {
        _ui = std::make_shared<UiVK>();

        _testBox = std::make_shared<TestBox>();
        _testBox->setPosition( 0.0f, 0.0f, -2.5f );

        _run = true;
    } catch ( const std::runtime_error& e ) {
        log::fatal()( "Exception thrown while creating render:\n{}", e.what() );
    }
}

auto RenderVK::isRun() -> bool {
    //
    return _run;
}

auto RenderVK::attachScene( std::shared_ptr<Scene> scene ) -> void {
    if ( _scene.get() ) {
        log::fatal()( "Can not attach scene twice!" );
    }

    _scene = std::move( scene );
}

}  // namespace tire