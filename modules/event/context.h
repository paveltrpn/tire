
#pragma once

#include <string>
#include <memory>
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_EVENT_CONTEXT_H{ true };

#include "../render/render.h"

#include <uv.h>

namespace tire::event {

struct Context final {
    Context() {
        if ( !instance_ ) {
            loop_ = static_cast<uv_loop_t *>( malloc( sizeof( uv_loop_t ) ) );

            const auto res = uv_loop_init( loop_ );
            if ( res != 0 ) {
                throw std::runtime_error(
                    std::format( "uv loop init failed with code {}", res ) );
            } else {
                log::debug<DEBUG_OUTPUT_EVENT_CONTEXT_H>(
                    "event::Context === uv loop init success" );
            }

            instance_.reset( this );
        } else {
            log::warning(
                "event::Context == attempt to reinitialize Context instance!" );
        }
    }

    Context( const Context &rhs ) = delete;
    Context( Context &&rhs ) = delete;
    Context &operator=( const Context &rhs ) = delete;
    Context &operator=( Context &&rhs ) = delete;

    ~Context() {
        uv_loop_close( loop_ );
        free( loop_ );
    };

    static Context *instance() {
        if ( !instance_ ) {
            log::error(
                "event::Context == global instance must be initialized "
                "explicitly!" );
        }
        return instance_.get();
    }

    static uv_loop_t *loop() { return instance()->getLoop(); }

    static void render( tire::Render *renderContext, uv_idle_cb cb ) {
        instance()->render_impl( renderContext, cb );
    }
    static void run() { instance()->run_impl(); }
    static void stop() { instance()->stop_impl(); }

private:
    [[nodiscard]] uv_loop_t *getLoop() const { return loop_; };

    void render_impl( tire::Render *renderContext, uv_idle_cb cb ) {
        uv_idle_init( loop_, &renderIdler_ );

        // Every libuv handle has a void* data field. Here’s how you use it:
        renderIdler_.data = renderContext;

        {
            const auto res = uv_idle_start( &renderIdler_, cb );
            if ( res != 0 ) {
                throw std::runtime_error(
                    std::format( "uv idle start failed with code {}", res ) );
            } else {
                log::debug<DEBUG_OUTPUT_EVENT_CONTEXT_H>(
                    "event::Context === uv idle started" );
            }
        }
    }

    void run_impl() {
        const auto res = uv_run( loop_, UV_RUN_DEFAULT );
        if ( res != 0 ) {
            log::warning( "event::Context === uv run end with code {}", res );
        }
    }

    void stop_impl() {
        // This will stop uv event loop and may
        // cause uv_run() return code != 1 if
        // not all event pendings got finished
        uv_stop( loop_ );
    }

private:
    static std::unique_ptr<Context> instance_;
    uv_loop_t *loop_{ nullptr };
    uv_idle_t renderIdler_;
};

}  // namespace tire::event
