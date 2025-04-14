
#pragma once

#include <chrono>
#include <limits>

namespace tire {

struct Timer final {
    Timer();

    Timer( const Timer &other ) = delete;
    Timer( Timer &&other ) = delete;

    Timer &operator=( const Timer &other ) = delete;
    Timer &operator=( Timer &&other ) = delete;

    ~Timer() = default;

    /*
     * @return - frame time in microseconds
    **/
    [[nodiscard]]
    long long frameDuration() const {
        return frameDuration_;
    }

    [[nodiscard]]
    float floatFrameDuration() const {
        return static_cast<float>( frameDuration_ ) * 0.000001f;
    };

    [[nodiscard]]
    double doubleFrameDuration() const {
        return static_cast<double>( frameDuration_ ) * 0.000001;
    };

    template <typename T>
        requires std::is_same_v<float, T> ||
        std::is_same_v<double, T> [[nodiscard]]
        T frameDuration() {
        return static_cast<T>( frameDuration_ ) * static_cast<T>( 0.000001 );
    }

    [[nodiscard]]
    long long averageFrameDuration() const {
        return frameDurationAccumulator_ / frameCountAccumulator_;
    }

    [[nodiscard]]
    float framesPerSecond() const {
        return static_cast<float>( frameCountAccumulator_ ) /
               ( static_cast<float>( frameDurationAccumulator_ ) * 0.000001f );
    }

    [[nodiscard]]
    std::array<long long, 2> minMaxFrameDuration() const {
        return { minFrameDuration_, maxFrameDuration_ };
    }

    void resetAccumulators() {
        //
        frameDurationAccumulator_ = frameCountAccumulator_ = 0;

        //
        minFrameDuration_ = std::numeric_limits<long long>::max();
        maxFrameDuration_ = 0;
    }

    // Called every frame
    void update();

private:
    std::chrono::steady_clock timer_;
    std::chrono::time_point<decltype( timer_ )> lastTimePoint_{};
    // In microseconds - 1/1000000s
    long long frameDuration_{};

    long long frameCountAccumulator_{};
    long long frameDurationAccumulator_{};
    long long minFrameDuration_{ std::numeric_limits<long long>::max() };
    long long maxFrameDuration_{};
};

}  // namespace tire