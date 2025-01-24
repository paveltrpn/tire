
#pragma once

#include <coroutine>
#include <exception>
#include <uv.h>

#include "context.h"

namespace tire::event {

template <typename T>
struct Task;

template <>
struct Task<void> {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        Task get_return_object() { return {}; };

        auto initial_suspend() noexcept { return std::suspend_never{}; }
        auto final_suspend() noexcept { return std::suspend_never{}; }
        void return_void() {}
        void unhandled_exception() { exception_ = std::current_exception(); }

        std::exception_ptr exception_{ nullptr };
    };
};

template <typename T>
struct TimeoutAwaitable final {
    TimeoutAwaitable( uv_loop_t *loop, long long timeout )
        : loop_( loop )
        , timeout_{ timeout } {};

    [[nodiscard]] bool await_ready() const noexcept { return false; }

    // can be void, bool, coroutine_handle<>
    auto await_suspend(
        std::coroutine_handle<typename T::promise_type> handle ) noexcept {
        auto cb = []( uv_timer_t *timer ) {
            auto handle =
                static_cast<std::coroutine_handle<typename T::promise_type> *>(
                    timer->data );
            handle->resume();
        };
        handle_ = handle;
        timer_.data = &handle_;
        uv_timer_init( loop_, &timer_ );
        uv_timer_start( &timer_, cb, timeout_, 0 );
    }

    void await_resume() const noexcept {}

    std::coroutine_handle<typename T::promise_type> handle_;
    uv_loop_t *loop_;
    long long timeout_;
    uv_timer_t timer_;
};

}  // namespace tire::event
