
module;

#include <cstddef>
#include <iostream>

#include <array>

export module algebra:matrix;

import :concepts;
import :vector;

namespace tire::algebra {

export template <typename T, size_t row, size_t column>
    requires Algebraic<T>
struct matrix_base {
    using self = matrix_base<T, row, column>;
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    [[nodiscard]] reference operator[]( size_t index ) { return _data[index]; }

    [[nodiscard]] const_reference operator[]( size_t index ) const {
        return _data[index];
    }

    [[nodiscard]] size_t size() { return _data.size(); }

    [[nodiscard]] pointer data() { return _data.data(); }

    [[nodiscard]] const_pointer data() const { return _data.data(); }

    [[nodiscard]] consteval size_t rows() { return row; };

    [[nodiscard]] consteval size_t columnes() { return column; };

protected:
    std::array<T, row * column> _data;
};

template <typename T, size_t rng>
    requires Algebraic<T>
struct matrix_sqr_base : matrix_base<T, rng, rng> {
    using base_type = matrix_base<T, rng, rng>;
    using self = matrix_sqr_base<T, rng>;
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    using base_type::_data;

    [[nodiscard]] consteval size_t range() { return rng; };

    void transpose() {
        value_type tmp;
        auto rt = *this;

        for ( size_t i = 0; i < rng; ++i ) {
            for ( size_t j = 0; j < i; ++j ) {
                tmp = rt[j, i];
                rt[j, i] = rt[i, j];
                rt[i, j] = tmp;
            }
        }

        *this = rt;
    }

    std::pair<self, self> lu() {
        self lm, um;
        size_t i, j, k;
        T sum;

        for ( i = 0; i < rng; i++ ) {
            for ( k = i; k < rng; k++ ) {
                sum = 0;
                for ( j = 0; j < i; j++ ) {
                    sum += ( lm[i, j] * um[j, k] );
                }
                um[i, k] = ( *this )[i, k] - sum;
            }

            for ( k = i; k < rng; k++ ) {
                if ( i == k ) {
                    lm[i, i] = T{ 1 };
                } else {
                    sum = 0;
                    for ( j = 0; j < i; j++ ) {
                        sum += lm[k, j] * um[j, i];
                    }
                    lm[k, i] = ( ( *this )[k, i] - sum ) / um[i, i];
                }
            }
        }

        return { lm, um };
    }

    value_type determinant_lu() {
        T det{ 1 };

        auto [_, u] = lu();
        for ( size_t i = 0; i < rng; i++ ) {
            det *= u[i, i];
        }

        return det;
    }
};

namespace detail {

template <typename T, size_t pSize_>
void matrix_sqr_set_zero( matrix_sqr_base<T, pSize_>& a ) {
    for ( size_t i = 0; i < pSize_; i++ ) {
        for ( size_t j = 0; j < pSize_; j++ ) {
            a[i, j] = T{};
        }
    }
}

template <typename T, size_t pSize_>
void matrix_sqr_set_idtt( matrix_sqr_base<T, pSize_>& a ) {
    for ( size_t i = 0; i < pSize_; i++ ) {
        for ( size_t j = 0; j < pSize_; j++ ) {
            if ( i == j ) {
                a[i, j] = static_cast<T>( 1.0 );
            } else {
                a[i, j] = T{};
            }
        }
    }
}

template <typename T, size_t pSize_>
auto matrix_sqr_mult( const matrix_sqr_base<T, pSize_>& a,
                      const matrix_sqr_base<T, pSize_>& b ) {
    matrix_sqr_base<T, pSize_> rt{};

    for ( size_t i = 0; i < pSize_; i++ ) {
        for ( size_t j = 0; j < pSize_; j++ ) {
            for ( size_t k = 0; k < pSize_; k++ ) {
                rt[i, j] += a[k, j] * b[i, k];
            }
        }
    }

    return rt;
}

template <typename T, size_t pSize_>
auto matrix_sqr_sum( const matrix_sqr_base<T, pSize_>& a,
                     const matrix_sqr_base<T, pSize_>& b ) {
    matrix_sqr_base<T, pSize_> rt{};

    for ( size_t i = 0; i < pSize_; i++ ) {
        for ( size_t j = 0; j < pSize_; j++ ) {
            rt[i, j] = a[i, j] + b[i, j];
        }
    }

    return rt;
}

template <typename T, size_t pSize_>
auto matrix_sqr_sub( const matrix_sqr_base<T, pSize_>& a,
                     const matrix_sqr_base<T, pSize_>& b ) {
    matrix_sqr_base<T, pSize_> rt{};

    for ( size_t i = 0; i < pSize_; i++ ) {
        for ( size_t j = 0; j < pSize_; j++ ) {
            rt[i, j] = a[i, j] - b[i, j];
        }
    }

    return rt;
}

template <typename T, size_t pSize_>
auto matrix_sqr_transpose( const matrix_sqr_base<T, pSize_>& m ) {
    T tmp;
    auto rt = m;

    for ( size_t i = 0; i < pSize_; ++i ) {
        for ( size_t j = 0; j < i; ++j ) {
            tmp = rt[i, j];
            rt[i, j] = rt[j, i];
            rt[j, i] = tmp;
        }
    }

    return rt;
}

template <typename T, size_t pSize_>
auto matrix_sqr_multVec( const matrix_sqr_base<T, pSize_>& m,
                         const vector_base<T, pSize_>& v ) {
    std::decay_t<decltype( m )> rt;

    for ( size_t i = 0; i < pSize_; i++ ) {
        for ( size_t j = 0; j < pSize_; j++ ) {
            rt[i] += m[i, j] * v[j];
        }
    }

    return rt;
}

template <typename T, size_t pSize_>
std::pair<matrix_sqr_base<T, pSize_>, matrix_sqr_base<T, pSize_>> matrix_sqr_lu(
    const matrix_sqr_base<T, pSize_>& m ) {
    std::decay_t<decltype( m )> lm, um;
    size_t i, j, k;
    T sum;

    for ( i = 0; i < pSize_; i++ ) {
        for ( k = i; k < pSize_; k++ ) {
            sum = 0;
            for ( j = 0; j < i; j++ ) {
                sum += ( lm[i, j] * um[j, k] );
            }
            um[i, k] = m[i, k] - sum;
        }

        for ( k = i; k < pSize_; k++ ) {
            if ( i == k ) {
                lm[i, i] = T{ 1 };
            } else {
                sum = 0;
                for ( j = 0; j < i; j++ ) {
                    sum += lm[k, j] * um[j, i];
                }
                lm[k, i] = ( m[k, i] - sum ) / um[i, i];
            }
        }
    }

    return { lm, um };
}

template <typename T, size_t pSize_>
T matrix_sqr_det_lu( const matrix_sqr_base<T, pSize_>& m ) {
    T det{ 1 };

    auto [_, u] = __matrix_sqr_lu( m );
    for ( size_t i = 0; i < pSize_; i++ ) {
        det *= u[i, i];
    }

    return det;
}

template <typename T, size_t pSize_>
std::pair<matrix_sqr_base<T, pSize_>, matrix_sqr_base<T, pSize_>>
matrix_sqr_ldlt( const matrix_sqr_base<T, pSize_>& m ) {
    std::decay_t<decltype( m )> lm;
    vector_base<T, pSize_> dv;
    size_t i, j, k;
    T sum;

    for ( i = 0; i < pSize_; i++ ) {
        for ( j = i; j < pSize_; j++ ) {
            sum = m[j, i];
            for ( k = 0; k < i; k++ ) {
                sum = sum - lm[i, k] * dv[k] * lm[j, k];
                if ( i == j ) {
                    if ( sum <= 0 ) {
                        std::cout << "matrix_sqr_ldlt(): matrix is not "
                                     "positive deﬁnite";
                        return { matrix_sqr_base<T, pSize_>{},
                                 vector_base<T, pSize_>{} };
                    }
                    dv[i] = sum;
                    lm[i, i] = 1.0;
                } else {
                    lm[j, i] = sum / dv[i];
                }
            }
        }
    }

    return { lm, dv };
}

template <typename T, size_t pSize_>
vector_base<T, pSize_> matrix_sqr_solve_gauss(
    const matrix_sqr_base<T, pSize_>& m, const vector_base<T, pSize_>& v ) {
    size_t i, j, k;
    T t;
    std::array<std::array<T, pSize_ + 1>, pSize_> a;
    vector_base<T, pSize_> rt;

    for ( i = 0; i < pSize_; i++ ) {
        for ( j = 0; j < pSize_; j++ ) {
            a[i][j] = m[i, j];
            a[i][pSize_] = v[i];
        }
    }

    /* Pivotisation */
    for ( i = 0; i < pSize_; i++ ) {
        for ( k = i + 1; k < pSize_; k++ ) {
            if ( fabs( a[i][i] ) < fabs( a[k][i] ) ) {
                for ( j = 0; j <= pSize_; j++ ) {
                    t = a[i][j];
                    a[i][j] = a[k][j];
                    a[k][j] = t;
                }
            }
        }
    }

    /* прямой ход */
    for ( k = 1; k < pSize_; k++ ) {
        for ( j = k; j < pSize_; j++ ) {
            t = a[j][k - 1] / a[k - 1][k - 1];
            for ( i = 0; i < pSize_ + 1; i++ ) {
                a[j][i] = a[j][i] - t * a[k - 1][i];
            }
        }
    }

    /* обратный ход */
    for ( i = pSize_ - 1; i > 0; i-- ) {
        rt[i] = a[i][pSize_] / a[i][i];
        for ( j = pSize_ - 1; j > i; j-- ) {
            rt[i] = rt[i] - a[i][j] * rt[j] / a[i][i];
        }
    }

    return rt;
}

template <typename T, size_t pSize_>
void matrix_sqr_insert_cmn( matrix_sqr_base<T, pSize_>& m,
                            const matrix_sqr_base<T, pSize_>& v ) {
    size_t i, j = 0;

    auto rt = m;

    for ( i = pSize_; i < pSize_ * pSize_; i += pSize_ ) {
        rt[i] = v[j];
        j++;
    }

    m = rt;
}

template <typename T, size_t pSize_>
vector_base<T, pSize_> matrix_sqr_solve_kramer(
    const matrix_sqr_base<T, pSize_>& m, const vector_base<T, pSize_>& v ) {
    T det;
    std::decay_t<decltype( m )> kr_mtrx;
    vector_base<T, pSize_> rt;

    det = matrix_sqr_det_lu( m );

    if ( fabs( det ) < std::numeric_limits<float>::epsilon() ) {
        std::cout << "matrix_sqr_solve_kramer(): system has no solve\n";
        return vector_base<T, pSize_>{};
    }

    for ( size_t i = 0; i < pSize_; i++ ) {
        matrix_sqr_insert_cmn( kr_mtrx, v, i );
        rt[i] = matrix_sqr_det_lu( kr_mtrx ) / det;
    }

    return rt;
}

}  // namespace detail

}  // namespace tire::algebra
