
#include <iostream>
#include <memory>
#include <memory>
#include <format>

#include <uv.h>

#include "timer.h"

namespace tire::io {

auto Timer::handlesInfo() const -> void {
    const auto total = handlesCount();
    int active{ 0 };

    pool_.for_each( [&active]( std::shared_ptr<TimerHandle> item ) -> void {
        if ( uv_is_active(
                 reinterpret_cast<const uv_handle_t*>( item.get() ) ) ) {
            ++active;
        }
    } );

    std::cout << std::format( "timers info: {}/{} (total/active)", total,
                              active );
}

auto Timer::handlesCount() const -> size_t {
    //
    return pool_.size();
}

auto Timer::timeout( uint64_t timeout ) -> TimeoutAwaitable<Task<void>> {
    //
    return { *this, timeout };
};

}  // namespace tire::io
