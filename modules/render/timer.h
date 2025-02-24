
#pragma once

#include <chrono>

namespace tire {

struct Timer final {
    Timer();

    Timer( const Timer &other ) = delete;
    Timer( Timer &&other ) = delete;

    Timer &operator=( const Timer &other ) = delete;
    Timer &operator=( Timer &&other ) = delete;

    ~Timer() = default;

    long long frameDuration() { return frameDuration_; }

    float floatFrameDuration() {
        return static_cast<float>( frameDuration_ ) / 1000000.0f;
    };

    double doubleFrameDuration() {
        return static_cast<double>( frameDuration_ ) / 1000000.0;
    };

    long long averageFrameDuration() {
        return frameTimeAccumulator_ / frameCount_;
    }

    void resetFrameMetrics() { frameTimeAccumulator_ = frameCount_ = 0; }

    // Called every frame
    void update();

private:
    std::chrono::steady_clock timer_;
    std::chrono::time_point<decltype( timer_ )> lastTimePoint_{};
    long long frameDuration_{};

    long long frameTimeAccumulator_{};
    long long frameCount_{};
};

}  // namespace tire