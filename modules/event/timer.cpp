
module;

#include <uv.h>
#include "log/log.h"

export module event:timer;
import :context;
import :async;

namespace tire::event {

export {
    void testTimerEvent( long long timeout ) {
        static uv_timer_t timer_req;
        auto cb = []( uv_timer_t* timer ) {
            log::notice( "test callback timer triggered!!!" );
        };
        uv_timer_init( Context::loop(), &timer_req );
        uv_timer_start( &timer_req, cb, timeout, 0 );
    }

    TimeoutAwaitable<Task<void>> setTimeout( long long timeout ) {
        return { Context::loop(), timeout };
    }
}

}  // namespace tire::event
