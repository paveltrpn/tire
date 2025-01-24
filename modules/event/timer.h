
#pragma once

#include <uv.h>

#include "async.h"

namespace tire::event {

void testTimerEvent( long long timeout );
TimeoutAwaitable<Task<void>> setTimeout( long long timeout );

}  // namespace tire::event
