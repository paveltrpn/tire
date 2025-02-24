
#include "timer.h"
#include <chrono>

namespace tire {

Timer::Timer() {
    lastTimePoint_ = timer_.now();
}

void Timer::update() {
    ++frameCount_;

    // Frame time calculation
    const auto newTimePoint_ = timer_.now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        newTimePoint_ - lastTimePoint_ );
    frameDuration_ = duration.count();
    lastTimePoint_ = newTimePoint_;

    // Will it overflow?
    frameTimeAccumulator_ += frameDuration_;
}

}  // namespace tire