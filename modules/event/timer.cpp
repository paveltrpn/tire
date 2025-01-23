
#include "context.h"
#include "timer.h"

namespace tire::event {

static void timerCallback( uv_timer_t *handle ) {
    log::info( "timer triggered..." );
}

void testTimerEvent( long long timeout ) {
    static uv_timer_t timer_req;
    uv_timer_init( Context::loop(), &timer_req );
    uv_timer_start( &timer_req, timerCallback, timeout, 0 );
}

}  // namespace tire::event
