
#pragma once

#include <string>

#include "uv.h"

namespace tire::io {

struct Factory final {
    Factory( uv_loop_t *loop )
        : loop_{ loop } {}

    void testFileOpen( std::string_view path );

private:
    // Non owning libuv main loop handle.
    // Used when "Factory" perform IO operations.
    uv_loop_t *loop_;
};

}  // namespace tire::io
