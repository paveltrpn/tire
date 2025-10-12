
module;

#include <string>
#include <memory>
#include "log/log.h"

#include "render/rendervk.h"

#include <uv.h>
#include <uv/version.h>

export module event:context;

static constexpr bool DEBUG_OUTPUT_EVENT_CONTEXT_H{ true };

namespace tire::event {

export struct Context final {
    Context() {
        if ( !instance_ ) {
            loop_ = static_cast<uv_loop_t*>( malloc( sizeof( uv_loop_t ) ) );

            const auto res = uv_loop_init( loop_ );
            if ( res != 0 ) {
                throw std::runtime_error(
                    std::format( "uv loop init failed with code {}", res ) );
            } else {
                log::info(
                    "event::Context === uv loop init success, libuv version "
                    "{}.{}",
                    UV_VERSION_MAJOR, UV_VERSION_MINOR );
            }

            instance_.reset( this );
        } else {
            log::warning(
                "event::Context == attempt to reinitialize Context "
                "instance!" );
        }
    }

    Context( const Context& rhs ) = delete;
    Context( Context&& rhs ) = delete;
    Context& operator=( const Context& rhs ) = delete;
    Context& operator=( Context&& rhs ) = delete;

    ~Context() {
        uv_loop_close( loop_ );
        free( loop_ );
    };

    static Context* instance() {
        if ( !instance_ ) {
            log::error(
                "event::Context == global instance must be initialized "
                "explicitly!" );
        }
        return instance_.get();
    }

    static uv_loop_t* loop() { return instance()->getLoop(); }

    static void render( tire::RenderVK* renderContext, uv_timer_cb cb ) {
        instance()->render_impl( renderContext, cb );
    }
    static void run() { instance()->run_impl(); }
    static void stop() { instance()->stop_impl(); }

    static void metrics() { instance()->metrics_impl(); }

private:
    [[nodiscard]] uv_loop_t* getLoop() const { return loop_; };

    void render_impl( tire::RenderVK* renderContext, uv_timer_cb cb ) {
        // Every libuv handle has a void* data field. Here’s how you use it:
        renderTimer_.data = renderContext;
        uv_timer_init( loop_, &renderTimer_ );
        {
// Use update interval that gives timer event more frequent then
// monitor refresh rate with vsync enable. In msec.
#define UPDATE_INTERVAL 10
            const auto res =
                uv_timer_start( &renderTimer_, cb, 0, UPDATE_INTERVAL );
            if ( res != 0 ) {
                throw std::runtime_error(
                    std::format( "uv idle start failed with code {}", res ) );
            } else {
                log::debug<DEBUG_OUTPUT_EVENT_CONTEXT_H>(
                    "event::Context === uv render idle start" );
            }
        }
    }

    void run_impl() {
        log::info( "event::Context === uv loop run..." );
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
        log::info( "event::Context === uv loop stopped" );
    }

    void metrics_impl() {
        uv_metrics_t metrics;
        uv_metrics_info( loop_, &metrics );
        log::info(
            "uv metrics:\nloop_count = {}\nevents = {}\nevents_waiting = {}",
            metrics.loop_count, metrics.events, metrics.events_waiting );
    }

private:
    static std::unique_ptr<Context> instance_;
    uv_loop_t* loop_{ nullptr };
    uv_timer_t renderTimer_;
};

std::unique_ptr<Context> Context::instance_ = nullptr;

}  // namespace tire::event
