
#pragma once

#include <cstddef>
#include <initializer_list>

#include "matrix.h"

namespace tire::algebra
{

template <typename T> struct matrix2 final : public matrix_sqr_base<T, 2> {
    using base_type = matrix_sqr_base<T, 2>;
    using typename base_type::value_type;
    using self = matrix2<value_type>;
    using base_type::_data;

    matrix2() {
        _data[0] = T{ 1 };
        _data[1] = T{};
        _data[2] = T{};
        _data[3] = T{ 1 };
    }

    matrix2( std::initializer_list<value_type> list ) {
        for ( auto i = 0; const auto e : list ) {
            _data[i] = e;
            ++i;
        }
    }

    void transpose() {
        value_type tmp;
        auto rt = *this;

        for ( size_t i = 0; i < 2; ++i ) {
            for ( size_t j = 0; j < i; ++j ) {
                tmp = rt[i, j];
                rt[i, j] = rt[j, i];
                rt[j, i] = tmp;
            }
        }

        *this = rt;
    }

    void multiply( const self &rhs ) {
        auto data0 = ( *this )[0];
        auto data1 = ( *this )[1];
        ( *this )[0] = rhs[0] * ( *this )[0] + rhs[1] * ( *this )[2];
        ( *this )[1] = rhs[0] * ( *this )[1] + rhs[1] * ( *this )[3];
        ( *this )[2] = rhs[2] * data0 + rhs[3] * ( *this )[2];
        ( *this )[3] = rhs[2] * data1 + rhs[3] * ( *this )[3];
    }

    self operator*( const self &rhs ) {
        auto tmp{ *this };
        tmp.multiply( rhs );
        *this = tmp;
        return *this;
    }

    value_type determinant() { return _data[0] * _data[3] - _data[2] * _data[1]; }
};

using matrix2l = matrix2<long long>;
using matrix2f = matrix2<float>;
using matrix2d = matrix2<double>;

} // namespace tire::algebra
