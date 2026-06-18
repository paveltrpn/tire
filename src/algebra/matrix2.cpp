
module;

#include <cstddef>
#include <initializer_list>
#include <array>

export module algebra:matrix2;

import :concepts;

namespace tire::algebra {

export template <typename T>
    requires Algebraic<T>
struct matrix2 final {
    using value_type = T;
    using self = matrix2<value_type>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    matrix2() {
        data_[0] = value_type{ 1 };
        data_[1] = value_type{ 0 };
        data_[2] = value_type{ 0 };
        data_[3] = value_type{ 1 };
    }

    matrix2( std::initializer_list<value_type> list ) {
        for ( auto i = 0; const auto e : list ) {
            data_[i] = e;
            ++i;
        }
    }

    [[nodiscard]]
    auto operator[]( size_t index ) -> reference {
        return data_[index];
    }

    [[nodiscard]]
    auto operator[]( size_t index ) const -> const_reference {
        return data_[index];
    }

    [[nodiscard]]
    auto size() -> size_t {
        //
        return data_.size();
    }

    [[nodiscard]]
    auto data() -> pointer {
        //
        return data_.data();
    }

    [[nodiscard]]
    auto data() const -> const_pointer {
        //
        return data_.data();
    }

    [[nodiscard]] auto transpose() -> self {
        value_type tmp;
        auto rt = *this;

        for ( size_t i = 0; i < 2; ++i ) {
            for ( size_t j = 0; j < i; ++j ) {
                tmp = rt[j, i];
                rt[j, i] = rt[i, j];
                rt[i, j] = tmp;
            }
        }

        return rt;
    }

    auto transposeSelf() -> void {
        value_type tmp;
        for ( size_t i = 0; i < 2; ++i ) {
            for ( size_t j = 0; j < i; ++j ) {
                tmp = ( *this )[j, i];
                ( *this )[j, i] = ( *this )[i, j];
                ( *this )[i, j] = tmp;
            }
        }
    }

    auto multiply( const self& rhs ) -> void {
        auto data0 = data_[0];
        auto data1 = data_[1];
        data_[0] = rhs[0] * data_[0] + rhs[1] * data_[2];
        data_[1] = rhs[0] * data_[1] + rhs[1] * data_[3];
        data_[2] = rhs[2] * data0 + rhs[3] * data_[2];
        data_[3] = rhs[2] * data1 + rhs[3] * data_[3];
    }

    [[nodiscard]]
    auto operator*( const self& rhs ) -> self {
        auto tmp{ *this };
        tmp.multiply( rhs );
        *this = tmp;
        return *this;
    }

    [[nodiscard]]
    auto determinant() -> value_type {
        return data_[0] * data_[3] - data_[2] * data_[1];
    }

private:
    std::array<value_type, 4> data_;
};

export using matrix2l = matrix2<long long>;
export using matrix2f = matrix2<float>;
export using matrix2d = matrix2<double>;

}  // namespace tire::algebra
