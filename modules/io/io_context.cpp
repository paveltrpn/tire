
module;

#include <vector>
#include <memory>
#include <coroutine>

#include <uv.h>

export module io:context;

import :event_scheduler;
import :async;
import :list;

namespace tire::io {

struct TimerHandle final {
    uv_timer_t handle_{};
    uint64_t timeout_{};
    uint64_t repeat_{};
    // uv_timer_cb type
    void ( *cb_ )( uv_timer_t* handle ){};
};

struct ReadHandle {
    uv_fs_t open_;
    uv_fs_t read_;
    uv_fs_t close_;
    uv_buf_t buf_;
};

struct WriteHandle {
    uv_fs_t open_;
    uv_fs_t write_;
    uv_fs_t close_;
    uv_buf_t buf_;
};

export template <typename T>
struct TimeoutAwaitable;

struct File final : EventScheduler {
private:
    std::vector<std::unique_ptr<ReadHandle>> readPool_{};
    std::vector<std::unique_ptr<WriteHandle>> writPool_{};
};

export struct IoContext final : EventScheduler {
    // Start timer repeating with interval.
    auto repeat( uint64_t repeat, void ( *cb )( uv_timer_t* ), void* payload )
        -> void {
        //
        auto t = std::make_shared<TimerHandle>();

        //
        t->timeout_ = 0;
        t->repeat_ = repeat;
        t->cb_ = cb;

        uv_handle_set_data( reinterpret_cast<uv_handle_t*>( &t->handle_ ),
                            payload );

        auto timerCb = []( uv_async_t* handle ) -> void {
            auto timer = static_cast<TimerHandle*>( handle->data );

            uv_timer_init( handle->loop, &timer->handle_ );
            uv_timer_start( &timer->handle_, timer->cb_, timer->timeout_,
                            timer->repeat_ );

            // Manually close active uv_async_t handle.
            // It exclude this handle from event loop queue.
            uv_close( reinterpret_cast<uv_handle_t*>( handle ), nullptr );
        };

        schedule( t.get(), timerCb );
    };

    // Fire once by timeout.
    auto timeout( uint64_t timeout, void ( *cb )( uv_timer_t* ), void* payload )
        -> void {
        //
        auto t = new TimerHandle{};

        //
        t->timeout_ = timeout;
        t->repeat_ = 0;
        t->cb_ = cb;

        uv_handle_set_data( reinterpret_cast<uv_handle_t*>( &t->handle_ ),
                            payload );

        auto timerCb = []( uv_async_t* handle ) -> void {
            auto timer = static_cast<TimerHandle*>( handle->data );

            uv_timer_init( handle->loop, &timer->handle_ );
            uv_timer_start( &timer->handle_, timer->cb_, timer->timeout_,
                            timer->repeat_ );

            uv_close( reinterpret_cast<uv_handle_t*>( handle ),
                      []( uv_handle_t* handle ) -> void {
                          //
                          delete handle;
                      } );
        };

        schedule( t, timerCb );
    };

    auto timeout( uint64_t timeout ) -> TimeoutAwaitable<Task<void>>;
};

template <typename T>
struct TimeoutAwaitable final {
    TimeoutAwaitable( IoContext& timer, uint64_t timeout )
        : timer_( timer )
        , timeout_{ timeout } {};

    [[nodiscard]]
    auto await_ready() const noexcept -> bool {
        //
        return false;
    }

    auto await_suspend(
        std::coroutine_handle<typename T::promise_type> handle ) noexcept {
        // Coroutine resume callback.
        auto cb = []( uv_timer_t* timer ) -> void {
            auto coroHandle =
                static_cast<std::coroutine_handle<typename T::promise_type>*>(
                    timer->data );
            coroHandle->resume();

            uv_close( reinterpret_cast<uv_handle_t*>( timer ),
                      []( uv_handle_t* handle ) -> void {
                          //
                          delete handle;
                      } );
        };

        coroHandle_ = handle;

        timer_.timeout( timeout_, cb, &coroHandle_ );
    }

    auto await_resume() const noexcept -> void {
        //
    }

    std::coroutine_handle<typename T::promise_type> coroHandle_;
    IoContext& timer_;
    uint64_t timeout_;
};

auto IoContext::timeout( uint64_t timeout ) -> TimeoutAwaitable<Task<void>> {
    //
    return { *this, timeout };
};

}  // namespace tire::io
