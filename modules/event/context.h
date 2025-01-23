
#pragma once

#include <string>
#include <memory>
#include "log/log.h"

#include "uv.h"

namespace tire::event {

struct Context final {
    Context( uv_loop_t *loop )
        : loop_{ loop } {
        if ( !instance_ ) {
            instance_.reset( this );
        } else {
            log::warning(
                "io::Context == attempt to reinitialize Context instance!" );
        }
    }

    Context( const Context &rhs ) = delete;
    Context( Context &&rhs ) = delete;
    Context &operator=( const Context &rhs ) = delete;
    Context &operator=( Context &&rhs ) = delete;

    ~Context() = default;

    static const Context *instance() {
        if ( !instance_ ) {
            log::error(
                "io::Context == global instance must be initialized "
                "explicitly!" );
        }
        return instance_.get();
    }

    static uv_loop_t *loop() { return instance()->getLoop(); }

private:
    [[nodiscard]] uv_loop_t *getLoop() const { return loop_; };

private:
    static std::unique_ptr<Context> instance_;

    // Non owning libuv main loop handle.
    // Used when "Context" perform IO operations.
    uv_loop_t *loop_;
};

}  // namespace tire::io
