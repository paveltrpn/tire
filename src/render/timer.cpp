
module;

#include <chrono>
#include <limits>

export module render:timer;

namespace tire {

struct Timer final {
    Timer() { lastTimePoint_ = timer_.now(); }

    Timer( const Timer& other ) = delete;
    Timer( Timer&& other ) = delete;

    Timer& operator=( const Timer& other ) = delete;
    Timer& operator=( Timer&& other ) = delete;

    ~Timer() = default;

#define TO_SECOND 0.000001

    /*
     * @return - frame time in microseconds
    **/
    [[nodiscard]]
    long long frameDuration() const {
        return frameDuration_;
    }

    [[nodiscard]]
    float floatFrameDuration() const {
        return static_cast<float>( frameDuration_ ) * TO_SECOND;
    };

    [[nodiscard]]
    double doubleFrameDuration() const {
        return static_cast<double>( frameDuration_ ) * TO_SECOND;
    };

    template <typename T>
        requires std::is_same_v<float, T> || std::is_same_v<double, T>
    [[nodiscard]]
    T frameDuration() {
        return static_cast<T>( frameDuration_ ) * static_cast<T>( TO_SECOND );
    }

    [[nodiscard]]
    long long averageFrameDuration() const {
        return frameDurationAccumulator_ / frameCountAccumulator_;
    }

    [[nodiscard]]
    float framesPerSecond() const {
        return static_cast<float>( frameCountAccumulator_ ) /
               ( static_cast<float>( frameDurationAccumulator_ ) * TO_SECOND );
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
    void update() {
        // Frame time calculation.
        // Time elapsed between two calls of this function.
        const auto newTimePoint_ = timer_.now();
        const auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(
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