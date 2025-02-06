module;

#include <string>
#include <coroutine>
#include <exception>
#include <uv.h>

export module event:async;

namespace tire::event {

export {
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
            void unhandled_exception() {
                exception_ = std::current_exception();
            }

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
                auto handle = static_cast<
                    std::coroutine_handle<typename T::promise_type> *>(
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

    template <typename T>
    struct FilesystemWatchAwaitable final {
        FilesystemWatchAwaitable( uv_loop_t *loop, const std::string &path )
            : loop_( loop )
            , path_{ path } {};

        [[nodiscard]] bool await_ready() const noexcept { return false; }

        auto await_suspend(
            std::coroutine_handle<typename T::promise_type> handle ) noexcept {
            auto cb = []( uv_fs_event_t *watcher, const char *filename,
                          int events, int status ) mutable {
                auto handle = static_cast<
                    std::coroutine_handle<typename T::promise_type> *>(
                    watcher->data );

                handle->resume();
            };
            handle_ = handle;
            watcher_.data = &handle_;
            uv_fs_event_init( loop_, &watcher_ );
            uv_fs_event_start( &watcher_, cb, path_.c_str(), 0 );
        }

        // NOTE: maybe const?
        void await_resume() noexcept {
            // Stop the handle, the callback will no longer be called.
            uv_fs_event_stop( &watcher_ );
        }

        std::coroutine_handle<typename T::promise_type> handle_;
        uv_loop_t *loop_;
        std::string path_;
        uv_fs_event_t watcher_;
    };
}
}  // namespace tire::event
