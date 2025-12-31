
module;

#include <functional>
#include <memory>
#include <coroutine>
#include <exception>
#include <print>

export module generator:generator;

namespace tire {

// Based on https://www.scs.stanford.edu/~dm/blog/c++-coroutines.html#generic-generator-example
export template <typename T>
struct generator {
    using value_type = T;

    struct promise_type;
    struct iterator;

    struct cleanup_callback {
        std::function<void()> cleanup;
    };

    struct cancel_control {
        std::shared_ptr<bool> &cancel;
    };

    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        T value_;
        std::exception_ptr exception_;
        std::function<void()> cleanup_;
        std::shared_ptr<bool> cancel_requested_;

        promise_type() {
            //
            cancel_requested_ = std::make_shared<bool>( false );
        }

        auto get_return_object() -> generator {
            //
            return generator( handle_type::from_promise( *this ) );
        }

        auto initial_suspend() -> std::suspend_always {
            //
            return {};
        }

        auto final_suspend() noexcept -> std::suspend_always {
            //
            return {};
        }

        auto unhandled_exception() -> void {
            //
            exception_ = std::current_exception();
        }

        template <typename From>
            requires std::is_convertible_v<value_type, From>
        auto yield_value( From &&from ) -> std::suspend_always {
            value_ = std::forward<From>( from );
            return {};
        }

        // overload the yield_value function
        // now we can pass a clean_up_callback to co_yield to set up cleanup callback
        auto yield_value( cleanup_callback &&token ) -> std::suspend_never {
            cleanup_ = token.cleanup;
            return {};
        }

        auto yield_value( cancel_control &&cancellation ) -> std::suspend_never {
            cancellation.cancel = cancel_requested_;
            return {};
        }

        auto return_void() -> void {
            //
        };
    };

    struct iterator : std::iterator<std::input_iterator_tag, value_type> {
        iterator( generator *g, size_t index = -1 )
            : index_{ index }
            , g_{ g } {}

        auto operator++() -> iterator & {
            if ( g_ != nullptr && g_->h_ != nullptr ) {
                g_->fill();
                if ( g_->h_.done() ) {
                    index_ = -1;
                } else {
                    index_++;
                }
            }
            return *this;
        }

        auto operator*() const -> value_type {
            //
            return std::move( g_->operator()() );
        }

        auto operator!=( const iterator &other ) const -> bool {
            //
            return this->g_ != other.g_ || index_ != other.index_;
        }

    public:
        generator *g_;
        size_t index_ = -1;
    };

    auto begin() -> iterator {
        if ( discarded_ || ( h_ && h_.done() ) ) return end();
        return iterator( this, 0 );
    }

    auto end() -> iterator {
        //
        return iterator( this );
    }

    generator( handle_type h )
        : h_( h ) {}

    ~generator() {
        //
        stop();
    }

    explicit operator bool() {
        if ( discarded_ ) {
            return false;
        }

        fill();

        return !h_.done();
    }

    auto operator()() -> value_type {
        if ( discarded_ ) {
            throw std::runtime_error( "Generator has been discarded." );
        }

        fill();

        full_ = false;

        return std::move( h_.promise().value_ );
    }

    // This function forces the coroutine to stop (without notifying it)
    auto stop() -> void {
        if ( !discarded_ ) {
            // check if the clean up function was assigned or not
            if ( h_.promise().cleanup_ ) {
                // do clean up
                h_.promise().cleanup_();
            }
            h_.destroy();
            discarded_ = true;
        }
    }

    // This function only requests the coroutine to stop instead of force it to stop
    auto request_stop() -> void {
        if ( !discarded_ ) {
            *h_.promise().cancel_requested = true;
        }
    }

private:
    void fill() {
        if ( !full_ && !discarded_ ) {
            num_evaluates++;
            h_();
            if ( h_.promise().exception_ ) {
                std::rethrow_exception( h_.promise().exception_ );
            }
            full_ = true;
        }
    }

public:
    handle_type h_;

    // Unused.
    size_t num_evaluates = 0;

private:
    bool discarded_ = false;
    bool full_ = false;
};

}  // namespace tire
