
module;

#include <type_traits>
#include <cstddef>
#include <mdspan>
#include <numbers>
#include <initializer_list>
#include <cmath>
#include <array>
#include <iostream>
#include <format>

export module algebra.matrix;

import algebra.vector;

namespace tire::algebra {

    template <std::floating_point T>
    constexpr T _half_circle = T(180.0L);

    template <std::floating_point T>
    constexpr T degToRad(T deg) {
        return deg * std::numbers::pi_v<T> / _half_circle<T>;
    }

    template <std::unsigned_integral T>
    constexpr size_t idRw(T i, size_t j, T n) {
        return (i * n + j);
    };

    template <std::unsigned_integral T>
    constexpr size_t idCw(T i, T j, T n) {
        return (j * n + i);
    };

    export template <typename T, size_t pWidth_, size_t pHeight_>
        requires(pWidth_ > 0 && pHeight_ > 0)
    struct matrix_base;

    export template <typename T, size_t pSize_>
    using matrix_base_sqr = matrix_base<T, pSize_, pSize_>;

    template <typename T, size_t pSize_>
    void __matrix_sqr_set_zero(matrix_base_sqr<T, pSize_>& a) {
        for (size_t i = 0; i < pSize_; i++) {
            for (size_t j = 0; j < pSize_; j++) {
                a[i, j] = static_cast<T>(0.0);
            }
        }
    }

    template <typename T, size_t pSize_>
    void __matrix_sqr_set_idtt(matrix_base_sqr<T, pSize_>& a) {
        for (size_t i = 0; i < pSize_; i++) {
            for (size_t j = 0; j < pSize_; j++) {
                if (i == j) {
                    a[i, j] = static_cast<T>(1.0);
                } else {
                    a[i, j] = T{};
                }
            }
        }
    }

    template <typename T, size_t pSize_>
    auto __matrix_sqr_mult(const matrix_base_sqr<T, pSize_>& a,
                           const matrix_base_sqr<T, pSize_>& b) {
        matrix_base<T, pSize_, pSize_> rt{};

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
    auto __matrix_sqr_sum(const matrix_base_sqr<T, pSize_>& a,
                          const matrix_base_sqr<T, pSize_>& b) {
        matrix_base<T, pSize_, pSize_> rt{};

        for (size_t i = 0; i < pSize_; i++) {
            for (size_t j = 0; j < pSize_; j++) {
                rt[i, j] = a[i, j] + b[i, j];
            }
        }

        return rt;
    }

    template <typename T, size_t pSize_>
    auto __matrix_sqr_sub(const matrix_base_sqr<T, pSize_>& a,
                          const matrix_base_sqr<T, pSize_>& b) {
        matrix_base<T, pSize_, pSize_> rt{};

        for (size_t i = 0; i < pSize_; i++) {
            for (size_t j = 0; j < pSize_; j++) {
                rt[i, j] = a[i, j] - b[i, j];
            }
        }

        return rt;
    }

    template <typename T, size_t pSize_>
    auto __matrix_sqr_transpose(const matrix_base_sqr<T, pSize_>& m) {
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
    auto __matrix_sqr_multVec(const matrix_base_sqr<T, pSize_>& m, const Vector<T, pSize_>& v) {
        std::decay_t<decltype(m)> rt;

        for (size_t i = 0; i < pSize_; i++) {
            for (size_t j = 0; j < pSize_; j++) {
                rt[i] += m[i, j] * v[j];
            }
        }

        return rt;
    }

    template <typename T, size_t pSize_>
    std::pair<matrix_base_sqr<T, pSize_>, matrix_base_sqr<T, pSize_>> __matrix_sqr_lu(
      const matrix_base_sqr<T, pSize_>& m) {
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
    T __matrix_sqr_det_lu(const matrix_base_sqr<T, pSize_>& m) {
        T det{ 1 };

        auto [_, u] = __matrix_sqr_lu(m);
        for (size_t i = 0; i < pSize_; i++) {
            det *= u[i, i];
        }

        return det;
    }

    template <typename T, size_t pSize_>
    std::pair<matrix_base_sqr<T, pSize_>, matrix_base_sqr<T, pSize_>> __matrix_sqr_ldlt(
      const matrix_base_sqr<T, pSize_>& m) {
        std::decay_t<decltype(m)> lm;
        Vector<T, pSize_> dv;
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
                            return { matrix_base<T, pSize_, pSize_>{}, Vector<T, pSize_>{} };
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
    Vector<T, pSize_> __matrix_sqr_solve_gauss(const matrix_base_sqr<T, pSize_>& m,
                                               const Vector<T, pSize_>& v) {
        size_t i, j, k;
        T t;
        std::array<std::array<T, pSize_ + 1>, pSize_> a;
        Vector<T, pSize_> rt;

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
    void __matrix_sqr_insert_cmn(matrix_base_sqr<T, pSize_>& m, const Vector<T, pSize_>& v) {
        size_t i, j = 0;

        auto rt = m;

        for (i = pSize_; i < pSize_ * pSize_; i += pSize_) {
            rt[i] = v[j];
            j++;
        }

        m = rt;
    }

    template <typename T, size_t pSize_>
    Vector<T, pSize_> __matrix_sqr_solve_kramer(const matrix_base_sqr<T, pSize_>& m,
                                                const Vector<T, pSize_>& v) {
        T det;
        std::decay_t<decltype(m)> kr_mtrx;
        Vector<T, pSize_> rt;

        det = __matrix_sqr_det_lu(m);

        if (fabs(det) < std::numeric_limits<float>::epsilon()) {
            std::cout << "__matrix_sqr_solve_kramer(): system has no solve\n";
            return Vector<T, pSize_>{};
        }

        for (size_t i = 0; i < pSize_; i++) {
            __matrix_sqr_insert_cmn(kr_mtrx, v, i);
            rt[i] = __matrix_sqr_det_lu(kr_mtrx) / det;
        }

        return rt;
    }

    export {
        template <typename T, size_t pWidth_, size_t pHeight_>
            requires(pWidth_ > 0 && pHeight_ > 0)
        struct matrix_base {
                using self = matrix_base<T, pWidth_, pHeight_>;
                using scalar_type = T;
                using reference = T&;
                using const_reference = const T&;
                using pointer = T*;
                using const_pointer = const T*;

                matrix_base() = default;
                matrix_base(const self& rhs) = default;
                matrix_base(self&& rhs) = default;

                self& operator=(const self& rhs) = default;
                self& operator=(self&& rhs) = default;

                ~matrix_base() = default;

                // row-wise indexing operator
                reference operator[](size_t i, size_t j) {
                    return std::mdspan(data_.data(), pWidth_, pHeight_)[i, j];
                }

            private:
                std::array<T, pWidth_ * pHeight_> data_;
        };

        template <typename T, size_t pSize_>
            requires(pSize_ > 0)
        struct matrix_base<T, pSize_, pSize_> {
                using self = matrix_base<T, pSize_, pSize_>;
                using scalar_type = T;
                using reference = T&;
                using const_reference = const T&;
                using pointer = T*;
                using const_pointer = const T*;

                static constexpr size_t size = pSize_;

                matrix_base() = default;
                matrix_base(const self& rhs) = default;
                matrix_base(self&& rhs) = default;

                self& operator=(const self& rhs) = default;
                self& operator=(self&& rhs) = default;

                ~matrix_base() = default;

                // row-wise indexing operator
                reference operator[](size_t i, size_t j) {
                    return std::mdspan(data_.data(), pSize_, pSize_)[i, j];
                }

                void idtt() {
                    __matrix_sqr_set_idtt(*this);
                }

                void mult(self b) {
                    auto tmp = __matrix_sqr_mult(*this, b);
                    *this = tmp;
                }

            private:
                std::array<T, pSize_ * pSize_> data_;
        };

        template <typename T>
        struct matrix_base<T, __SZ2, __SZ2> {
                using self = matrix_base<T, __SZ2, __SZ2>;
                using self_vec = vector2<T>;
                using scalar_type = T;
                using reference = T&;
                using const_reference = const T&;
                using pointer = T*;
                using const_pointer = const T*;

                static constexpr size_t size = __SZ2;

                matrix_base() = default;
                matrix_base(const self& rhs) = default;
                matrix_base(self&& rhs) = default;

                explicit matrix_base(std::initializer_list<scalar_type> list) {
                    zero();
                    for (size_t i = 0; auto&& elem : list) {
                        data_[i] = elem;
                        if (i > __SZ2 * __SZ2)
                            break;
                        ++i;
                    }
                }

                self& operator=(const self& rhs) = default;
                self& operator=(self&& rhs) = default;

                ~matrix_base() = default;

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

                void zero() {
                    __matrix_sqr_set_zero(*this);
                }

                void idtt() {
                    __matrix_sqr_set_idtt(*this);
                }

                self operator*(const self& rhs) {
                    return __matrix_sqr_mult(*this, rhs);
                }

                void mult(self b) {
                    auto tmp = __matrix_sqr_mult(*this, b);
                    *this = tmp;
                }

                std::pair<self, self> lu() {
                    return __matrix_sqr_lu(*this);
                }

                scalar_type det_lu() {
                    return __matrix_sqr_det_lu(*this);
                }

                std::pair<self, self> ldlt() {
                    return __matrix_sqr_ldlt(*this);
                }

                vector4_base<scalar_type> solve_gauss(const vector4_base<scalar_type>& v) {
                    return __matrix_sqr_solve_gauss(*this, v);
                }

                void insert_cmn(const vector4_base<scalar_type>& v) {
                    __matrix_sqr_insert_cmn(*this, v);
                }

                vector4_base<scalar_type> solve_kramer(const vector4_base<scalar_type>& v) {
                    return __matrix_sqr_solve_kramer(*this, v);
                }

                void transpose() {
                    std::swap(data_[2], data_[1]);
                }

            private:
                std::array<T, __SZ2 * __SZ2> data_;
        };

        template <typename T>
        struct matrix_base<T, __SZ3, __SZ3> {
                using self = matrix_base<T, __SZ3, __SZ3>;
                using self_vec = vector3_base<T>;
                using scalar_type = T;
                using reference = T&;
                using const_reference = const T&;
                using pointer = T*;
                using const_pointer = const T*;

                static constexpr size_t size = __SZ3;

                matrix_base() = default;
                matrix_base(const self& rhs) = default;
                matrix_base(self&& rhs) = default;

                explicit matrix_base(std::initializer_list<scalar_type> list) {
                    if (list.size() >= 9) {
                        for (size_t i = 0; auto&& elem : list) {
                            data_[i] = elem;
                            ++i;
                        }
                    } else if (list.size() < 9) {
                        zero();
                        for (size_t i = 0; auto&& elem : list) {
                            data_[i] = elem;
                            ++i;
                        }
                    }
                }

                self& operator=(const self& rhs) = default;
                self& operator=(self&& rhs) = default;

                ~matrix_base() = default;

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

                void zero() {
                    __matrix_sqr_set_zero(*this);
                }

                void idtt() {
                    __matrix_sqr_set_idtt(*this);
                }

                void mult(self b) {
                    auto tmp = __matrix_sqr_mult(*this, b);
                    *this = tmp;
                }

                self operator*(const self& rhs) {
                    return __matrix_sqr_mult(*this, rhs);
                }

                self_vec operator*(const self_vec& rhs) {
                    return this->mult_vec(rhs);
                }

                std::pair<self, self> lu() {
                    return __matrix_sqr_lu(*this);
                }

                scalar_type det_lu() {
                    return __matrix_sqr_det_lu(*this);
                }

                std::pair<self, self> ldlt() {
                    return __matrix_sqr_ldlt(*this);
                }

                vector4_base<scalar_type> solve_gauss(const vector4_base<scalar_type>& v) {
                    return __matrix_sqr_solve_gauss(*this, v);
                }

                void insert_cmn(const vector4_base<scalar_type>& v) {
                    __matrix_sqr_insert_cmn(*this, v);
                }

                vector4_base<scalar_type> solve_kramer(const vector4_base<scalar_type>& v) {
                    return __matrix_sqr_solve_kramer(*this, v);
                }

                void transpose() {
                    auto tmp = __matrix_sqr_transpose(*this);
                    *this = tmp;
                }

                void display() {
                    std::cout << std::format(
                      "{:.5f} {:.5f} {:.5f}\n{:5f} {:5f} {:5f}\n{:5f} {:5f} {:5f}\n",
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
        std::ostream& operator<<(std::ostream& os, matrix_base<T, __SZ3, __SZ3>& m) {
            os << std::format("{} {} {}\n{} {} {}\n{} {} {}\n",
                              m[0],
                              m[1],
                              m[2],
                              m[3],
                              m[4],
                              m[5],
                              m[6],
                              m[7],
                              m[8]);
            return os;
        }

        template <typename T>
        struct matrix_base<T, __SZ4, __SZ4> {
                using self = matrix_base<T, __SZ4, __SZ4>;
                using self_vec = vector4_base<T>;
                using scalar_type = T;
                using reference = T&;
                using const_reference = const T&;
                using pointer = T*;
                using const_pointer = const T*;

                static constexpr size_t size = __SZ4;

                matrix_base() = default;
                matrix_base(const self& rhs) = default;
                matrix_base(self&& rhs) = default;

                explicit matrix_base(std::initializer_list<scalar_type> list) {
                    zero();
                    for (size_t i = 0; auto&& elem : list) {
                        data_[i] = elem;
                        if (i > __SZ4 * __SZ4)
                            break;
                        ++i;
                    }
                }

                self& operator=(const self& rhs) = default;
                self& operator=(self&& rhs) = default;

                ~matrix_base() = default;

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

                void zero() {
                    __matrix_sqr_set_zero(*this);
                }

                void idtt() {
                    __matrix_sqr_set_idtt(*this);
                }

                void mult(self b) {
                    auto tmp = __matrix_sqr_mult(*this, b);
                    *this = tmp;
                }

                self operator*(const self& rhs) {
                    return __matrix_sqr_mult(*this, rhs);
                }

                self_vec operator*(const self_vec& rhs) {
                    return this->mult_vec(rhs);
                }

                std::pair<self, self> lu() {
                    return __matrix_sqr_lu(*this);
                }

                scalar_type det_lu() {
                    return __matrix_sqr_det_lu(*this);
                }

                std::pair<self, self> ldlt() {
                    return __matrix_sqr_ldlt(*this);
                }

                vector4_base<scalar_type> solve_gauss(const vector4_base<scalar_type>& v) {
                    return __matrix_sqr_solve_gauss(*this, v);
                }

                void insert_cmn(const vector4_base<scalar_type>& v) {
                    __matrix_sqr_insert_cmn(*this, v);
                }

                vector4_base<scalar_type> solve_kramer(const vector4_base<scalar_type>& v) {
                    return __matrix_sqr_solve_kramer(*this, v);
                }

                void transpose() {
                    auto tmp = __matrix_sqr_transpose(*this);
                    *this = tmp;
                }

                vector3_base<scalar_type> mult_vec3(const vector3_base<scalar_type>& v) {
                    vector3_base<scalar_type> rt;
                    scalar_type w;

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

                vector3_base<scalar_type> operator*(const vector3_base<scalar_type>& rhs) {
                    return this->mult_vec3(rhs);
                }

                void perspective(scalar_type fovy,
                                 scalar_type aspect,
                                 scalar_type near,
                                 scalar_type far) {
                    scalar_type h = std::cos(static_cast<scalar_type>(0.5) * fovy)
                                    / std::sin(static_cast<scalar_type>(0.5) * fovy);

                    data_[0] = h / aspect;
                    data_[1] = 0.0;
                    data_[2] = 0.0;
                    data_[3] = 0.0;
                    data_[4] = 0.0;
                    data_[5] = h;
                    data_[6] = 0.0;
                    data_[7] = 0.0;
                    data_[8] = 0.0;
                    data_[9] = 0.0;
                    data_[11] = -1.0;
                    data_[12] = 0.0;
                    data_[13] = 0.0;
                    data_[15] = 0.0;

                    if (far >= std::numeric_limits<scalar_type>::epsilon()) {
                        scalar_type nf;
                        nf = 1.0 / (far - near);
                        data_[10] = -(far + near) * nf;
                        data_[14] = -(2.0 * far * near) * nf;
                    } else {
                        data_[10] = -1.0;
                        data_[14] = -2.0 * near;
                    }
                }

                void lookAt(const vector3_base<scalar_type>& eye,
                            const vector3_base<scalar_type>& center,
                            const vector3_base<scalar_type>& up) {
                    idtt();
                    vector3_base<scalar_type> eyeDir;

                    constexpr scalar_type floatEps = std::numeric_limits<scalar_type>::epsilon();
                    if (std::fabs(eye[0] - center[0]) < floatEps
                        && std::fabs(eye[1] - center[1]) < floatEps
                        && std::fabs(eye[2] - center[2]) < floatEps) {
                        return;
                    }

                    scalar_type z0 = eye[0] - center[0];
                    scalar_type z1 = eye[1] - center[1];
                    scalar_type z2 = eye[2] - center[2];

                    scalar_type len = 1.0 / std::hypot(z0, z1, z2);  //??? было просто hypot
                    z0 *= len;
                    z1 *= len;
                    z2 *= len;

                    scalar_type x0 = up[1] * z2 - up[2] * z1;
                    scalar_type x1 = up[2] * z0 - up[0] * z2;
                    scalar_type x2 = up[0] * z1 - up[1] * z0;
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

                    scalar_type y0 = z1 * x2 - z2 * x1;
                    scalar_type y1 = z2 * x0 - z0 * x2;
                    scalar_type y2 = z0 * x1 - z1 * x0;

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

                void orthographic(scalar_type left,
                                  scalar_type right,
                                  scalar_type bottom,
                                  scalar_type top,
                                  scalar_type near,
                                  scalar_type far) {
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

                void scale(const vector3_base<scalar_type>& offset) {
                    idtt();

                    data_[0] = offset[0];
                    data_[5] = offset[1];
                    data_[11] = offset[2];
                }

                void translate(const vector3_base<scalar_type>& offset) {
                    idtt();

                    data_[3] = offset[0];
                    data_[7] = offset[1];
                    data_[11] = offset[2];
                }

                void translate(scalar_type dx, scalar_type dy, scalar_type dz) {
                    idtt();

                    data_[3] = dx;
                    data_[7] = dy;
                    data_[11] = dz;
                }

                void rotation_yaw(scalar_type angl) {
                    idtt();
                    scalar_type sa{}, ca{};

                    sincosf(degToRad(angl), &sa, &ca);

                    data_[5] = ca;
                    data_[6] = -sa;
                    data_[9] = sa;
                    data_[10] = ca;
                }

                void rotation_pitch(scalar_type angl) {
                    idtt();
                    scalar_type sa{}, ca{};

                    sincosf(degToRad(angl), &sa, &ca);

                    data_[0] = ca;
                    data_[2] = -sa;
                    data_[8] = sa;
                    data_[10] = ca;
                }

                void rotation_roll(scalar_type angl) {
                    idtt();
                    scalar_type sa{}, ca{};

                    sincosf(degToRad(angl), &sa, &ca);

                    data_[0] = ca;
                    data_[1] = -sa;
                    data_[4] = sa;
                    data_[5] = ca;
                }

                void euler(scalar_type yaw, scalar_type pitch, scalar_type roll) {
                    self y, p, r;

                    y.rotation_yaw(yaw);
                    p.rotation_pitch(pitch);
                    r.rotation_roll(roll);

                    *this = y * p * r;
                }

                void axis_angle(const vector3_base<scalar_type>& ax, scalar_type phi) {
                    scalar_type cosphi, sinphi, vxvy, vxvz, vyvz, vx, vy, vz;

                    cosphi = std::cos(degToRad(phi));
                    sinphi = std::sin(degToRad(phi));
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

        template <typename T, size_t size_>
        auto transpose(matrix_base<T, size_, size_> & arg) -> decltype(auto) {
            return __matrix_sqr_transpose(arg);
        }

        template <typename T>
        matrix_base<T, 4, 4> translate(T dx, T dy, T dz) {
            matrix_base<T, 4, 4> rt{};
            rt.translate(dx, dy, dz);
            return rt;
        }

        template <typename T>
        matrix_base<T, 4, 4> translate(vector3_base<T> & offset) {
            matrix_base<T, 4, 4> rt{};
            rt.translate(offset.x(), offset.y(), offset.z());
            return rt;
        }

        template <typename T>
        matrix_base<T, 4, 4> rotate(T dy, T dp, T dr) {
            matrix_base<T, 4, 4> rt;
            rt.euler(dy, dp, dr);
            return rt;
        }

        template <typename T>
        matrix_base<T, 4, 4> rotate(vector3_base<T> ax, T phi) {
            matrix_base<T, 4, 4> rt;
            rt.axis_angle(ax, phi);
            return rt;
        }

        template <typename T>
        matrix_base<T, 4, 4> perspective(float fov, float aspect, float ncp, float fcp) {
            matrix_base<T, 4, 4> rt;
            rt.perspective(fov, aspect, ncp, fcp);
            return rt;
        }

        template <typename T>
        matrix_base<T, 4, 4> orthographic(T left, T right, T bottom, T top, T near, T far) {
            matrix_base<T, 4, 4> rt;
            rt.orthographic(left, right, bottom, top, near, far);
            return rt;
        }

        template <typename T, size_t size_>
        using Matrix = matrix_base<T, size_, size_>;

        template <typename T>
        using Matrix2 = matrix_base<T, 2, 2>;

        template <typename T>
        using Matrix3 = matrix_base<T, 3, 3>;

        template <typename T>
        using Matrix4 = matrix_base<T, 4, 4>;

        using Matrix2i = Matrix2<int>;
        using Matrix3i = Matrix3<int>;
        using Matrix4i = Matrix4<int>;

        using Matrix2ui = Matrix2<unsigned int>;
        using Matrix3ui = Matrix3<unsigned int>;
        using Matrix4ui = Matrix4<unsigned int>;

        using Matrix2ll = Matrix2<long long>;
        using Matrix3ll = Matrix3<long long>;
        using Matrix4ll = Matrix4<long long>;

        using Matrix2f = Matrix2<float>;
        using Matrix3f = Matrix3<float>;
        using Matrix4f = Matrix4<float>;

        using Matrix2d = Matrix2<double>;
        using Matrix3d = Matrix3<double>;
        using Matrix4d = Matrix4<double>;

        template <typename T>
        concept ConceptMatrixFloat
          = std::is_same_v<T, Matrix<float, 2>> || std::is_same_v<T, Matrix<float, 3>>
            || std::is_same_v<T, Matrix<float, 4>>;

        template <typename T>
        concept ConceptMatrix
          = std::is_same_v<T, Matrix<long long, 4>> || std::is_same_v<T, Matrix<float, 4>>
            || std::is_same_v<T, Matrix<double, 4>> || std::is_same_v<T, Matrix<int, 4>>
            || std::is_same_v<T, Matrix<unsigned int, 4>> || std::is_same_v<T, Matrix<long long, 3>>
            || std::is_same_v<T, Matrix<float, 3>> || std::is_same_v<T, Matrix<double, 3>>
            || std::is_same_v<T, Matrix<int, 3>> || std::is_same_v<T, Matrix<unsigned int, 3>>
            || std::is_same_v<T, Matrix<long long, 2>> || std::is_same_v<T, Matrix<float, 2>>
            || std::is_same_v<T, Matrix<double, 2>> || std::is_same_v<T, Matrix<int, 2>>
            || std::is_same_v<T, Matrix<unsigned int, 2>>;
    }
}  // namespace tire::algebra

