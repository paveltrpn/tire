
#include "timer.h"
#include <chrono>

namespace tire {

Timer::Timer() {
    lastTimePoint_ = timer_.now();
}

void Timer::update() {
    // Frame time calculation.
    // Time elapsed between two calls of this function.
    const auto newTimePoint_ = timer_.now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        newTimePoint_ - lastTimePoint_ );
    frameDuration_ = duration.count();
    lastTimePoint_ = newTimePoint_;

    // Get minimal and maximal frametime
    if ( ( frameDuration_ - minFrameDuration_ ) < 0 ) {
        minFrameDuration_ = frameDuration_;
    } else if ( ( frameDuration_ - maxFrameDuration_ ) > 0 ) {
        maxFrameDuration_ = frameDuration_;
    }

    // Update accumulators.
    ++frameCountAccumulator_;
    // Will it overflow?
    frameDurationAccumulator_ += frameDuration_;
}

}  // namespace tire