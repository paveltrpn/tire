
#pragma once

#include <string>
#include <memory>
#include "log/log.h"

#include "uv.h"

namespace tire::io {

struct Factory final {
    Factory( uv_loop_t *loop )
        : loop_{ loop } {
        if ( !instance_ ) {
            instance_.reset( this );
        } else {
            log::warning(
                "io::Factory == attempt to reinitialize Factory instance!" );
        }
    }

    static const Factory *instance() {
        if ( !instance_ ) {
            log::error(
                "io::Factory == global instance must be initialized "
                "explicitly!" );
        }
        return instance_.get();
    }

    static uv_loop_t *loop() { return instance()->getLoop(); }

private:
    [[nodiscard]] uv_loop_t *getLoop() const { return loop_; };

private:
    static std::unique_ptr<Factory> instance_;

    // Non owning libuv main loop handle.
    // Used when "Factory" perform IO operations.
    uv_loop_t *loop_;
};

}  // namespace tire::io
