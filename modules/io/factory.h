
#pragma once

#include "uv.h"

namespace tire::io {

struct Factory final {
    Factory( const uv_loop_t *loop )
        : loop_{ loop } {}

private:
    // Non owning libuv main loop handle.
    // Used when "Factory" perform IO operations.
    const uv_loop_t *loop_;
};

}  // namespace tire::io
