
#pragma once

#include <thread>
#include <mutex>
#include "uv.h"
#include <condition_variable>

#include "list.h"

namespace tire::io {

struct EventScheduler {
    EventScheduler();

    // Not copyable not moveable "service-like" object.
    EventScheduler( const EventScheduler& other ) = delete;
    EventScheduler( EventScheduler&& other ) = delete;
    EventScheduler& operator=( const EventScheduler& other ) = delete;
    EventScheduler& operator=( EventScheduler&& other ) = delete;

    virtual ~EventScheduler();

    auto syncWait() -> void;

protected:
    // Add new uv_async_t handle in queue and submit callback to event loop within it.
    // That async handle contain a pointer to specific data, that will be
    // used in callback and callback to perform action on event loop thread itself.
    auto schedule( void* payload, std::invocable<uv_async_t*> auto cb ) -> void;

private:
    // Main and only uv loop handle.
    uv_loop_t* loop_{};

    // Loop thread.
    std::unique_ptr<std::thread> thread_;

    //
    std::condition_variable cv_;
    std::mutex m_;
    bool run_{ false };

    // Async handles that used for calling callbacks
    // on loop thread.
    list<uv_async_t> pendingQueue_{};
};

}  // namespace tire::io