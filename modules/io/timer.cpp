
#include "factory.h"
#include "timer.h"

namespace tire::io {

static void timerCallback( uv_timer_t *handle ) {
    log::info( "timer triggered..." );
}

void testTimerEvent( long long timeout ) {
    static uv_timer_t timer_req;
    uv_timer_init( Factory::loop(), &timer_req );
    uv_timer_start( &timer_req, timerCallback, timeout, 0 );
}

}  // namespace tire::io
