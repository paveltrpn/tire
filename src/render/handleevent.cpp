
#include "rendervk.h"

#include "log/log.h"

namespace tire {

auto RenderVK::handleEvent( std::unique_ptr<EventBase> event ) -> void {
    auto keyPress = dynamic_cast<EventKeyPress*>( event.get() );
    if ( keyPress ) {
        log::notice()( "render catch a key press event" );
    }
};

}  // namespace tire
