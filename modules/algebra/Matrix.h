
#ifndef __algebramatrix_h__
#define __algebramatrix_h__

#include <iostream>
#include <cstddef>
#include <mdspan>
#include <numbers>
#include <initializer_list>
#include <type_traits>
#include <format>
#include <print>

#include "Vector.h"

namespace tire {

template <typename T>
constexpr T degToRad(T deg) {
    return deg * M_PI / 180.0;
}

template <std::unsigned_integral T>
constexpr size_t idRw(T i, size_t j, T n) {
    return (i * n + j);
};

template <std::unsigned_integral T>
constexpr size_t idCw(T i, T j, T n) {
    return (j * n + i);
};

template <typename T, size_t pWidth_, size_t pHeight_>
    requires(pWidth_ > 0 && pHeight_ > 0)
struct matrix;

template <typename T>
using matrix2 = matrix<T, __SZ2, __SZ2>;

template <typename T>
using matrix3 = matrix<T, __SZ3, __SZ3>;

template <typename T>
using matrix4 = matrix<T, __SZ4, __SZ4>;

using matrix2f = matrix2<float>;
using matrix3f = matrix3<float>;
using matrix4f = matrix4<float>;

using matrix2d = matrix2<double>;
using matrix3d = matrix3<double>;
using matrix4d = matrix4<double>;

template <typename T, size_t pSize_>
using matrix_sqr = matrix<T, pSize_, pSize_>;

template <typename T, size_t pSize_>
static inline void __matrix_sqr_set_zero(matrix_sqr<T, pSize_>& a) {
    for (size_t i = 0; i < pSize_; i++) {
        for (size_t j = 0; j < pSize_; j++) {
            a[i, j] = static_cast<T>(0);
        }
    }
}

template <typename T, size_t pSize_>
static inline void __matrix_sqr_set_idtt(matrix_sqr<T, pSize_>& a) {
    for (size_t i = 0; i < pSize_; i++) {
        for (size_t j = 0; j < pSize_; j++) {
            if (i == j) {
                a[i, j] = static_cast<T>(1);
            } else {
                a[i, j] = T{};
            }
        }
    }
}

template <typename T, size_t pSize_>
static inline auto __matrix_sqr_mult(const matrix_sqr<T, pSize_>& a,
                                     const matrix_sqr<T, pSize_>& b) {
    matrix<T, pSize_, pSize_> rt{};

    for (size_t i = 0; i < pSize_; i++) {
        for (size_t j = 0; j < pSize_; j++) {
            for (size_t k = 0; k < pSize_; k++) {
                rt[i, j] += a[k, j] * b[i, k];
            }
        }
    }

    return rt;
}

template <typename T, size_t pSize_>
static inline auto __matrix_sqr_sum(const matrix_sqr<T, pSize_>& a,
                                    const matrix_sqr<T, pSize_>& b) {
    matrix<T, pSize_, pSize_> rt{};

    for (size_t i = 0; i < pSize_; i++) {
        for (size_t j = 0; j < pSize_; j++) {
            rt[i, j] = a[i, j] + b[i, j];
        }
    }

    return rt;
}

template <typename T, size_t pSize_>
static inline auto __matrix_sqr_sub(const matrix_sqr<T, pSize_>& a,
                                    const matrix_sqr<T, pSize_>& b) {
    matrix<T, pSize_, pSize_> rt{};

    for (size_t i = 0; i < pSize_; i++) {
        for (size_t j = 0; j < pSize_; j++) {
            rt[i, j] = a[i, j] - b[i, j];
        }
    }

    return rt;
}

template <typename T, size_t pSize_>
static inline auto __matrix_sqr_transpose(const matrix_sqr<T, pSize_>& m) {
    T tmp;
    auto rt = m;

    for (size_t i = 0; i < pSize_; ++i) {
        for (size_t j = 0; j < i; ++j) {
            tmp = rt[i, j];
            rt[i, j] = rt[j, i];
            rt[j, i] = tmp;
        }
    }

    return rt;
}

template <typename T, size_t pSize_>
static inline auto __matrix_sqr_multVec(const matrix_sqr<T, pSize_>& m,
                                        const vector<T, pSize_>& v) {
    std::decay_t<decltype(m)> rt;

    for (size_t i = 0; i < pSize_; i++) {
        for (size_t j = 0; j < pSize_; j++) {
            rt[i] += m[i, j] * v[j];
        }
    }

    return rt;
}

template <typename T, size_t pSize_>
static inline std::pair<matrix_sqr<T, pSize_>, matrix_sqr<T, pSize_>> __matrix_sqr_lu(
  const matrix_sqr<T, pSize_>& m) {
    std::decay_t<decltype(m)> lm, um;
    size_t i, j, k;
    T sum;

    for (i = 0; i < pSize_; i++) {
        for (k = i; k < pSize_; k++) {
            sum = 0;
            for (j = 0; j < i; j++) {
                sum += (lm[i, j] * um[j, k]);
            }
            um[i, k] = m[i, k] - sum;
        }

        for (k = i; k < pSize_; k++) {
            if (i == k) {
                lm[i, i] = T{ 1 };
            } else {
                sum = 0;
                for (j = 0; j < i; j++) {
                    sum += lm[k, j] * um[j, i];
                }
                lm[k, i] = (m[k, i] - sum) / um[i, i];
            }
        }
    }

    return { lm, um };
}

template <typename T, size_t pSize_>
static inline T __matrix_sqr_det_lu(const matrix_sqr<T, pSize_>& m) {
    T det{ 1 };

    auto [_, u] = __matrix_sqr_lu(m);
    for (size_t i = 0; i < pSize_; i++) {
        det *= u[i, i];
    }

    return det;
}

template <typename T, size_t pSize_>
static inline std::pair<matrix_sqr<T, pSize_>, matrix_sqr<T, pSize_>> __matrix_sqr_ldlt(
  const matrix_sqr<T, pSize_>& m) {
    std::decay_t<decltype(m)> lm;
    vector<T, pSize_> dv;
    size_t i, j, k;
    T sum;

    for (i = 0; i < pSize_; i++) {
        for (j = i; j < pSize_; j++) {
            sum = m[j, i];
            for (k = 0; k < i; k++) {
                sum = sum - lm[i, k] * dv[k] * lm[j, k];
                if (i == j) {
                    if (sum <= 0) {
                        std::cout << "__matrix_sqr_ldlt(): matrix is not positive deﬁnite";
                        return { matrix<T, pSize_, pSize_>{}, vector<T, pSize_>{} };
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
static inline vector<T, pSize_> __matrix_sqr_solve_gauss(const matrix_sqr<T, pSize_>& m,
                                                         const vector<T, pSize_>& v) {
    size_t i, j, k;
    T t;
    std::array<std::array<T, pSize_ + 1>, pSize_> a;
    vector<T, pSize_> rt;

    for (i = 0; i < pSize_; i++) {
        for (j = 0; j < pSize_; j++) {
            a[i][j] = m[i, j];
            a[i][pSize_] = v[i];
        }
    }

    /* Pivotisation */
    for (i = 0; i < pSize_; i++) {
        for (k = i + 1; k < pSize_; k++) {
            if (fabs(a[i][i]) < fabs(a[k][i])) {
                for (j = 0; j <= pSize_; j++) {
                    t = a[i][j];
                    a[i][j] = a[k][j];
                    a[k][j] = t;
                }
            }
        }
    }

    /* прямой ход */
    for (k = 1; k < pSize_; k++) {
        for (j = k; j < pSize_; j++) {
            t = a[j][k - 1] / a[k - 1][k - 1];
            for (i = 0; i < pSize_ + 1; i++) {
                a[j][i] = a[j][i] - t * a[k - 1][i];
            }
        }
    }

    /* обратный ход */
    for (i = pSize_ - 1; i > 0; i--) {
        rt[i] = a[i][pSize_] / a[i][i];
        for (j = pSize_ - 1; j > i; j--) {
            rt[i] = rt[i] - a[i][j] * rt[j] / a[i][i];
        }
    }

    return rt;
}

template <typename T, size_t pSize_>
static inline void __matrix_sqr_insert_cmn(matrix_sqr<T, pSize_>& m, const vector<T, pSize_>& v) {
    size_t i, j = 0;

    auto rt = m;

    for (i = pSize_; i < pSize_ * pSize_; i += pSize_) {
        rt[i] = v[j];
        j++;
    }

    m = rt;
}

template <typename T, size_t pSize_>
static inline vector<T, pSize_> __matrix_sqr_solve_kramer(const matrix_sqr<T, pSize_>& m,
                                                          const vector<T, pSize_>& v) {
    T det;
    std::decay_t<decltype(m)> kr_mtrx;
    vector<T, pSize_> rt;

    det = __matrix_sqr_det_lu(m);

    if (fabs(det) < std::numeric_limits<float>::epsilon()) {
        std::cout << "__matrix_sqr_solve_kramer(): system has no solve\n";
        return vector<T, pSize_>{};
    }

    for (size_t i = 0; i < pSize_; i++) {
        __matrix_sqr_insert_cmn(kr_mtrx, v, i);
        rt[i] = __matrix_sqr_det_lu(kr_mtrx) / det;
    }

    return rt;
}

template <typename T, size_t pWidth_, size_t pHeight_>
    requires(pWidth_ > 0 && pHeight_ > 0)
struct matrix {
        using self = matrix<T, pWidth_, pHeight_>;
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;

        matrix() = default;
        matrix(const self& rhs) = default;
        matrix(self&& rhs) = default;

        self& operator=(const self& rhs) = default;
        self& operator=(self&& rhs) = default;

        ~matrix() = default;

        // row-wise indexing operator
        reference operator[](size_t i, size_t j) {
            return std::mdspan(data_.data(), pWidth_, pHeight_)[i, j];
        }

    private:
        std::array<T, pWidth_ * pHeight_> data_;
};

template <typename T, size_t pSize_>
    requires(pSize_ > 0)
struct matrix<T, pSize_, pSize_> {
        using self = matrix<T, pSize_, pSize_>;
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;

        matrix() = default;
        matrix(const self& rhs) = default;
        matrix(self&& rhs) = default;

        self& operator=(const self& rhs) = default;
        self& operator=(self&& rhs) = default;

        ~matrix() = default;

        // row-wise indexing operator
        reference operator[](size_t i, size_t j) {
            return std::mdspan(data_.data(), pSize_, pSize_)[i, j];
        }

        void set_idtt() {
            __matrix_sqr_set_idtt(*this);
        }

        self mult(self b) {
            return __matrix_sqr_mult(*this, b);
        }

    private:
        std::array<T, pSize_ * pSize_> data_;
};

template <typename T>
struct matrix<T, __SZ2, __SZ2> {
        using self = matrix<T, __SZ2, __SZ2>;
        using self_vec = vector2<T>;
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;

        matrix() = default;
        matrix(const self& rhs) = default;
        matrix(self&& rhs) = default;

        explicit matrix(std::initializer_list<value_type> list) {
            set_zero();
            for (size_t i = 0; auto&& elem : list) {
                data_[i] = elem;
                if (i > __SZ4 * __SZ4)
                    break;
                ++i;
            }
        }

        self& operator=(const self& rhs) = default;
        self& operator=(self&& rhs) = default;

        ~matrix() = default;

        reference operator[](size_t id) {
            return data_[id];
        }

        const_reference operator[](size_t id) const {
            return data_[id];
        }

        // row-wise indexing operator
        reference operator[](size_t i, size_t j) {
            return std::mdspan(data_.data(), __SZ2, __SZ2)[i, j];
        }

        const_reference operator[](size_t i, size_t j) const {
            return std::mdspan(data_.data(), __SZ2, __SZ2)[i, j];
        }

        void set_zero() {
            __matrix_sqr_set_zero(*this);
        }

        void set_idtt() {
            __matrix_sqr_set_idtt(*this);
        }

        self mult(self b) {
            return __matrix_sqr_mult(*this, b);
        }

        self operator*(const self& rhs) {
            return this->mult(rhs);
        }

        self sum(self b) {
            return __matrix_sqr_sum(*this, b);
        }

        self sub(self b) {
            return __matrix_sqr_sub(*this, b);
        }

        self_vec mult_vec(const self_vec& rhs) {
            return __matrix_sqr_multVec(*this, rhs);
        }

        self_vec operator*(const self_vec& rhs) {
            return this->mult_vec(rhs);
        }

        void mult_self(self b) {
            auto tmp = __matrix_sqr_mult(*this, b);
            *this = tmp;
        }

        std::pair<self, self> lu() {
            return __matrix_sqr_lu(*this);
        }

        value_type det_lu() {
            return __matrix_sqr_det_lu(*this);
        }

        std::pair<self, self> ldlt() {
            return __matrix_sqr_ldlt(*this);
        }

        vector4<value_type> solve_gauss(const vector4<value_type>& v) {
            return __matrix_sqr_solve_gauss(*this, v);
        }

        void insert_cmn(const vector4<value_type>& v) {
            __matrix_sqr_insert_cmn(*this, v);
        }

        vector4<value_type> solve_kramer(const vector4<value_type>& v) {
            return __matrix_sqr_solve_kramer(*this, v);
        }

        self transpose() {
            return __matrix_sqr_transpose(*this);
        }

        void transpose_self() {
            std::swap(data_[2], data_[1]);
        }

    private:
        std::array<T, __SZ2 * __SZ2> data_;
};

template <typename T>
struct matrix<T, __SZ3, __SZ3> {
        using self = matrix<T, __SZ3, __SZ3>;
        using self_vec = vector3<T>;
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;

        matrix() = default;
        matrix(const self& rhs) = default;
        matrix(self&& rhs) = default;

        explicit matrix(std::initializer_list<value_type> list) {
            if (list.size() >= 9) {
                for (size_t i = 0; auto&& elem : list) {
                    data_[i] = elem;
                    ++i;
                }
            } else if (list.size() < 9) {
                set_zero();
                for (size_t i = 0; auto&& elem : list) {
                    data_[i] = elem;
                    ++i;
                }
            }
        }

        self& operator=(const self& rhs) = default;
        self& operator=(self&& rhs) = default;

        ~matrix() = default;

        reference operator[](size_t id) {
            return data_[id];
        }

        const_reference operator[](size_t id) const {
            return data_[id];
        }

        // row-wise indexing operator
        reference operator[](size_t i, size_t j) {
            return std::mdspan(data_.data(), __SZ3, __SZ3)[i, j];
        }

        const_reference operator[](size_t i, size_t j) const {
            return std::mdspan(data_.data(), __SZ3, __SZ3)[i, j];
        }

        void set_zero() {
            __matrix_sqr_set_zero(*this);
        }

        void set_idtt() {
            __matrix_sqr_set_idtt(*this);
        }

        self mult(const self& b) {
            return __matrix_sqr_mult(*this, b);
        }

        self operator*(const self& rhs) {
            return this->mult(rhs);
        }

        self sum(const self& b) {
            return __matrix_sqr_sum(*this, b);
        }

        self sub(const self& b) {
            return __matrix_sqr_sub(*this, b);
        }

        self_vec mult_vec(const self_vec& rhs) {
            return __matrix_sqr_multVec(*this, rhs);
        }

        self_vec operator*(const self_vec& rhs) {
            return this->mult_vec(rhs);
        }

        void mult_self(const self& b) {
            auto tmp = __matrix_sqr_mult(*this, b);
            *this = tmp;
        }

        std::pair<self, self> lu() {
            return __matrix_sqr_lu(*this);
        }

        value_type det_lu() {
            return __matrix_sqr_det_lu(*this);
        }

        std::pair<self, self> ldlt() {
            return __matrix_sqr_ldlt(*this);
        }

        vector4<value_type> solve_gauss(const vector4<value_type>& v) {
            return __matrix_sqr_solve_gauss(*this, v);
        }

        void insert_cmn(const vector4<value_type>& v) {
            __matrix_sqr_insert_cmn(*this, v);
        }

        vector4<value_type> solve_kramer(const vector4<value_type>& v) {
            return __matrix_sqr_solve_kramer(*this, v);
        }

        self transpose() {
            return __matrix_sqr_transpose(*this);
        }

        void display() {
            std::cout << std::format("{:.5f} {:.5f} {:.5f}\n{:5f} {:5f} {:5f}\n{:5f} {:5f} {:5f}\n",
                                     data_[0],
                                     data_[1],
                                     data_[2],
                                     data_[3],
                                     data_[4],
                                     data_[5],
                                     data_[6],
                                     data_[7],
                                     data_[8]);
        }

    private:
        std::array<T, __SZ3 * __SZ3> data_;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, matrix<T, __SZ3, __SZ3>& m) {
    os << std::format(
      "{} {} {}\n{} {} {}\n{} {} {}\n", m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8]);
    return os;
}

template <typename T>
struct matrix<T, __SZ4, __SZ4> {
        using self = matrix<T, __SZ4, __SZ4>;
        using self_vec = vector4<T>;
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;

        matrix() = default;
        matrix(const self& rhs) = default;
        matrix(self&& rhs) = default;

        explicit matrix(std::initializer_list<value_type> list) {
            set_zero();
            for (size_t i = 0; auto&& elem : list) {
                data_[i] = elem;
                if (i > __SZ4 * __SZ4)
                    break;
                ++i;
            }
        }

        self& operator=(const self& rhs) = default;
        self& operator=(self&& rhs) = default;

        ~matrix() = default;

        reference operator[](size_t id) {
            return data_[id];
        }

        const_reference operator[](size_t id) const {
            return data_[id];
        }

        // row-wise indexing operator
        reference operator[](size_t i, size_t j) {
            return std::mdspan(data_.data(), __SZ4, __SZ4)[i, j];
        }

        const_reference operator[](size_t i, size_t j) const {
            return std::mdspan(data_.data(), __SZ4, __SZ4)[i, j];
        }

        void set_zero() {
            __matrix_sqr_set_zero(*this);
        }

        void set_idtt() {
            __matrix_sqr_set_idtt(*this);
        }

        self mult(self b) {
            return __matrix_sqr_mult(*this, b);
        }

        self operator*(const self& rhs) {
            return this->mult(rhs);
        }

        self sum(self b) {
            return __matrix_sqr_sum(*this, b);
        }

        self sub(self b) {
            return __matrix_sqr_sub(*this, b);
        }

        self_vec mult_vec(const self_vec& rhs) {
            return __matrix_sqr_multVec(*this, rhs);
        }

        self_vec operator*(const self_vec& rhs) {
            return this->mult_vec(rhs);
        }

        void mult_self(self b) {
            auto tmp = __matrix_sqr_mult(*this, b);
            *this = tmp;
        }

        std::pair<self, self> lu() {
            return __matrix_sqr_lu(*this);
        }

        value_type det_lu() {
            return __matrix_sqr_det_lu(*this);
        }

        std::pair<self, self> ldlt() {
            return __matrix_sqr_ldlt(*this);
        }

        vector4<value_type> solve_gauss(const vector4<value_type>& v) {
            return __matrix_sqr_solve_gauss(*this, v);
        }

        void insert_cmn(const vector4<value_type>& v) {
            __matrix_sqr_insert_cmn(*this, v);
        }

        vector4<value_type> solve_kramer(const vector4<value_type>& v) {
            return __matrix_sqr_solve_kramer(*this, v);
        }

        self transpose() {
            return __matrix_sqr_transpose(*this);
        }

        vector3<value_type> mult_vec3(const vector3<value_type>& v) {
            vector3<value_type> rt;
            value_type w;

            rt[0] = v[0] * *this[0] + v[1] * *this[1] + v[2] * *this[2] + *this[3];
            rt[1] = v[0] * *this[4] + v[1] * *this[5] + v[2] * *this[6] + *this[7];
            rt[2] = v[0] * *this[8] + v[1] * *this[9] + v[2] * *this[10] + *this[11];
            w = v[0] * *this[12] + v[1] * *this[13] + v[2] * *this[14] + *this[15];

            // normalize if w is different than 1 (convert from homogeneous to Cartesian
            // coordinates)
            if (w != 1.0f) {
                rt[0] /= w;
                rt[1] /= w;
                rt[2] /= w;
            }

            return rt;
        }

        vector3<value_type> operator*(const vector3<value_type>& rhs) {
            return this->mult_vec3(rhs);
        }

        void set_perspective(value_type fovy, value_type aspect, value_type near, value_type far) {
            value_type f = 1.0 / tanf(fovy / std::numbers::sqrt2_v<float>);
            value_type nf;

            data_[0] = f / aspect;
            data_[1] = 0.0;
            data_[2] = 0.0;
            data_[3] = 0.0;
            data_[4] = 0.0;
            data_[5] = f;
            data_[6] = 0.0;
            data_[7] = 0.0;
            data_[8] = 0.0;
            data_[9] = 0.0;
            data_[11] = -1.0;
            data_[12] = 0.0;
            data_[13] = 0.0;
            data_[15] = 0.0;

            if (far >= std::numeric_limits<value_type>::epsilon()) {
                nf = 1.0 / (near - far);
                data_[10] = (far + near) * nf;
                data_[14] = 2.0 * far * near * nf;
            } else {
                data_[10] = -1.0;
                data_[14] = -2.0 * near;
            }
        }

        void set_lookAt(const vector3<value_type>& eye,
                        const vector3<value_type>& center,
                        const vector3<value_type>& up) {
            set_idtt();
            vector3<value_type> eyeDir;

            constexpr value_type floatEps = std::numeric_limits<value_type>::epsilon();
            if (std::fabs(eye[0] - center[0]) < floatEps && std::fabs(eye[1] - center[1]) < floatEps
                && std::fabs(eye[2] - center[2]) < floatEps) {
                return;
            }

            value_type z0 = eye[0] - center[0];
            value_type z1 = eye[1] - center[1];
            value_type z2 = eye[2] - center[2];

            value_type len = 1.0 / std::hypot(z0, z1, z2);  //??? было просто hypot
            z0 *= len;
            z1 *= len;
            z2 *= len;

            value_type x0 = up[1] * z2 - up[2] * z1;
            value_type x1 = up[2] * z0 - up[0] * z2;
            value_type x2 = up[0] * z1 - up[1] * z0;
            len = std::hypot(x0, x1, x2);
            if (len == 0.0) {
                x0 = 0;
                x1 = 0;
                x2 = 0;
            } else {
                len = 1.0 / len;
                x0 *= len;
                x1 *= len;
                x2 *= len;
            }

            value_type y0 = z1 * x2 - z2 * x1;
            value_type y1 = z2 * x0 - z0 * x2;
            value_type y2 = z0 * x1 - z1 * x0;

            len = std::hypot(y0, y1, y2);
            if (len == 0.0) {
                y0 = 0;
                y1 = 0;
                y2 = 0;
            } else {
                len = 1.0 / len;
                y0 *= len;
                y1 *= len;
                y2 *= len;
            }

            data_[0] = x0;
            data_[1] = y0;
            data_[2] = z0;
            data_[3] = 0.0;
            data_[4] = x1;
            data_[5] = y1;
            data_[6] = z1;
            data_[7] = 0.0;
            data_[8] = x2;
            data_[9] = y2;
            data_[10] = z2;
            data_[11] = 0.0;
            data_[12] = -(x0 * eye[0] + x1 * eye[1] + x2 * eye[2]);
            data_[13] = -(y0 * eye[0] + y1 * eye[1] + y2 * eye[2]);
            data_[14] = -(z0 * eye[0] + z1 * eye[1] + z2 * eye[2]);
            data_[15] = 1.0;
        }

        void set_orthographic(value_type left,
                              value_type right,
                              value_type bottom,
                              value_type top,
                              value_type near,
                              value_type far) {
            data_[0] = 2.0 / (right - left);
            data_[1] = 0;
            data_[2] = 0;
            data_[3] = 0;

            data_[4] = 0;
            data_[5] = 2.0 / (top - bottom);
            data_[6] = 0;
            data_[7] = 0;

            data_[8] = 0;
            data_[9] = 0;
            data_[10] = -2.0 / (far - near);
            data_[11] = 0;

            data_[12] = -(right + left) / (right - left);
            data_[13] = -(top + bottom) / (top - bottom);
            data_[14] = -(far + near) / (far - near);
            data_[15] = 1.0;
        }

        void set_scale(const vector3<value_type>& offset) {
            this->set_idtt();

            data_[0] = offset[0];
            data_[5] = offset[1];
            data_[11] = offset[2];
        }

        void set_offset(const vector3<value_type>& offset) {
            this->set_idtt();

            data_[3] = offset[0];
            data_[7] = offset[1];
            data_[11] = offset[2];
        }

        void transpose_self() {
            self tmp;

            tmp[0] = data_[0];
            tmp[1] = data_[4];
            tmp[2] = data_[8];
            tmp[3] = data_[12];
            tmp[4] = data_[1];
            tmp[5] = data_[5];
            tmp[6] = data_[9];
            tmp[7] = data_[13];
            tmp[8] = data_[2];
            tmp[9] = data_[6];
            tmp[10] = data_[10];
            tmp[11] = data_[14];
            tmp[12] = data_[3];
            tmp[13] = data_[7];
            tmp[14] = data_[11];
            tmp[15] = data_[15];

            *this = tmp;
        }

        void set_rotation_yaw(value_type angl) {
            this->set_idtt();
            value_type sa, ca;

            sincosf(degToRad(angl), &sa, &ca);

            data_[5] = ca;
            data_[6] = -sa;
            data_[9] = sa;
            data_[10] = ca;
        }

        void set_rotation_pitch(value_type angl) {
            this->set_idtt();
            float sa, ca;

            sincosf(degToRad(angl), &sa, &ca);

            data_[0] = ca;
            data_[2] = sa;
            data_[8] = -sa;
            data_[10] = ca;
        }

        void set_rotation_roll(value_type angl) {
            this->set_idtt();
            float sa, ca;

            sincosf(degToRad(angl), &sa, &ca);

            data_[0] = ca;
            data_[1] = -sa;
            data_[4] = sa;
            data_[5] = ca;
        }

        void set_euler(value_type yaw, value_type pitch, value_type roll) {
            self y, p, r;

            y.set_rotation_yaw(yaw);
            p.set_rotation_pitch(pitch);
            r.set_rotation_roll(roll);

            *this = y * p;
            mult_self(r);
        }

        void set_axis_angl(const vector3<value_type>& ax, value_type phi) {
            value_type cosphi, sinphi, vxvy, vxvz, vyvz, vx, vy, vz;

            cosphi = cosf(degToRad(phi));
            sinphi = sinf(degToRad(phi));
            vxvy = ax[0] * ax[1];
            vxvz = ax[0] * ax[2];
            vyvz = ax[1] * ax[2];
            vx = ax[0];
            vy = ax[1];
            vz = ax[2];

            data_[0] = cosphi + (1.0 - cosphi) * vx * vx;
            data_[1] = (1.0 - cosphi) * vxvy - sinphi * vz;
            data_[2] = (1.0 - cosphi) * vxvz + sinphi * vy;
            data_[3] = 0.0f;

            data_[4] = (1.0 - cosphi) * vxvy + sinphi * vz;
            data_[5] = cosphi + (1.0 - cosphi) * vy * vy;
            data_[6] = (1.0 - cosphi) * vyvz - sinphi * vx;
            data_[7] = 0.0f;

            data_[8] = (1.0 - cosphi) * vxvz - sinphi * vy;
            data_[9] = (1.0 - cosphi) * vyvz + sinphi * vx;
            data_[10] = cosphi + (1.0 - cosphi) * vz * vz;
            data_[11] = 0.0f;

            data_[12] = 0.0f;
            data_[13] = 0.0f;
            data_[14] = 0.0f;
            data_[15] = 1.0f;
        }

        void display() {
            std::cout << std::format("{:.5f} {:.5f} {:.5f} {:5f}\n"
                                     "{ : .5f } { : .5f } { : .5f } { : 5f }\n"
                                     "{ : .5f } { : .5f } { : .5f } { : 5f }\n"
                                     "{ : .5f } { : .5f } { : .5f } { : 5f }\n",
                                     data_[0],
                                     data_[1],
                                     data_[2],
                                     data_[3],
                                     data_[4],
                                     data_[5],
                                     data_[6],
                                     data_[7],
                                     data_[8],
                                     data_[9],
                                     data_[10],
                                     data_[11],
                                     data_[12],
                                     data_[13],
                                     data_[14],
                                     data_[15]);
        }

        [[nodiscard]] pointer data() {
            return data_.data();
        }

        [[nodiscard]] const_pointer data() const {
            return data_.data();
        }

    private:
        std::array<T, __SZ4 * __SZ4> data_;
};

namespace algebra {

template <typename T, size_t size_>
struct matrix_base_glm {
        using scalar_type = T;
        using self = matrix_base_glm<T, size_>;

    private:
        glm::mat<size_, size_, scalar_type> mat_;
};

template <typename T>
struct matrix_base_glm<T, 2> {
        using scalar_type = T;
        using vec_type = Vector<scalar_type, 2>;
        using self = matrix_base_glm<scalar_type, 2>;

        matrix_base_glm() = default;
        matrix_base_glm(vec_type r1, vec_type r2)
            : mat_{ glm::vec<2, scalar_type>{ r1.x(), r1.y() },
                    glm::vec<2, scalar_type>{ r2.x(), r2.y() } } {
        }

    private:
        glm::mat<2, 2, scalar_type> mat_;
};

template <typename T>
struct matrix_base_glm<T, 3> {
        using scalar_type = T;
        using vec_type = Vector<scalar_type, 3>;
        using self = matrix_base_glm<scalar_type, 3>;

        matrix_base_glm() = default;
        matrix_base_glm(vec_type r1, vec_type r2, vec_type r3)
            : mat_{ glm::vec<3, scalar_type>{ r1.x(), r1.y(), r1.x() },
                    glm::vec<3, scalar_type>{ r2.x(), r2.y(), r1.z() },
                    glm::vec<3, scalar_type>{ r3.x(), r3.y(), r3.z() } } {
        }

    private:
        glm::mat<3, 3, scalar_type> mat_;
};

template <typename T>
struct matrix_base_glm<T, 4> {
        using scalar_type = T;
        using vec_type = Vector<scalar_type, 3>;
        using self = matrix_base_glm<scalar_type, 4>;

        matrix_base_glm() = default;
        matrix_base_glm(vec_type r1, vec_type r2, vec_type r3, vec_type r4)
            : mat_{ glm::vec<4, scalar_type>{ r1.x(), r1.y(), r1.x(), r1.w() },
                    glm::vec<4, scalar_type>{ r2.x(), r2.y(), r1.z(), r2.w() },
                    glm::vec<4, scalar_type>{ r3.x(), r3.y(), r3.z(), r3.w() },
                    glm::vec<4, scalar_type>{ r4.x(), r4.y(), r4.z(), r4.w() } } {
        }

    private:
        glm::mat<4, 4, scalar_type> mat_;
};

template <typename T, size_t size_>
using Matrix = matrix_base_glm<T, size_>;

template <typename T>
using Matrix2 = matrix_base_glm<T, 2>;

template <typename T>
using Matrix3 = matrix_base_glm<T, 3>;

template <typename T>
using Matrix4 = matrix_base_glm<T, 4>;

using Matrix2i = Matrix2<long long>;
using Matrix3i = Matrix3<long long>;
using Matrix4i = Matrix4<long long>;

using Matrix2f = Matrix2<float>;
using Matrix3f = Matrix3<float>;
using Matrix4f = Matrix4<float>;

using Matrix2d = Matrix2<double>;
using Matrix3d = Matrix3<double>;
using Matrix4d = Matrix4<double>;

}  // namespace algebra

}  // namespace tire

#endif
