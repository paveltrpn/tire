
#include <cstdlib>
#include <uv.h>

#include "event_scheduler.h"

namespace tire::io {

EventScheduler::EventScheduler() {
    // Try to allocate main loop handle.
    const auto tmp = std::malloc( sizeof( uv_loop_t ) );
    if ( !tmp ) {
        throw std::bad_alloc{};
    }

    loop_ = static_cast<uv_loop_t*>( tmp );

    // We not yet in loop thread. Calling this is safe.
    const auto ret = uv_loop_init( loop_ );

    // Loop thread initialization.
    thread_ = std::make_unique<std::thread>( [this]() {
        // Main thread must explicitly call notify this
        // thread to start event loop.
        std::unique_lock<std::mutex> lk{ m_ };
        cv_.wait( lk, [this]() -> bool {
            //
            return run_;
        } );

        // Start event loop.
        uv_run( loop_, UV_RUN_DEFAULT );
    } );
}

EventScheduler::~EventScheduler() {
    // We already left loop thread.
    // De-initialize and free the resources associated with an event loop.
    uv_loop_close( loop_ );

    // Release loop pointer.
    free( loop_ );
}

auto EventScheduler::syncWait() -> void {
    {
        std::lock_guard<std::mutex> _{ m_ };
        run_ = true;
    }
    cv_.notify_one();

    //
    thread_->join();
}

auto EventScheduler::schedule( void* payload,
                               std::invocable<uv_async_t*> auto cb ) -> void {
    // Allocate uv_async_t handle
    auto j = std::make_shared<uv_async_t>();

    // Store payload pointer in async handle
    j->data = payload;

    // Initilaize async handle with specific payload and callback.
    // NOTE: this is (probably) not thread safe!
    uv_async_init( loop_, j.get(), cb );

    // Submit job to event loop.
    // This thread safe (one and only thread safe call across
    // all lobuv API)
    uv_async_send( j.get() );

    // Store handle.
    pendingQueue_.append( std::move( j ) );
}

}  // namespace tire::io
