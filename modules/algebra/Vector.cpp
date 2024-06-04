
module;

#include <cstddef>
#include <cmath>
#include <array>

export module algebra:vector;

namespace tire::algebra {

    constexpr size_t __SZ2 = 2;
    constexpr size_t __SZ3 = 3;
    constexpr size_t __SZ4 = 4;

    template <typename T>
    struct lenght_type {
            using type = float;
    };

    template <>
    struct lenght_type<int> {
            using type = float;
    };

    template <>
    struct lenght_type<double> {
            using type = double;
    };

    template <>
    struct lenght_type<long double> {
            using type = long double;
    };

    template <template <typename> typename derived, typename T, size_t pSize_>
    struct vector_base {
            using self = vector_base<derived, T, pSize_>;
            using scalar_type = T;
            using reference = T&;
            using pointer = T*;
            using const_pointer = const T*;

            scalar_type& operator[](size_t id) {
                return data_[id];
            }

            void sum(const self& b) {
                for (size_t i = 0; i < pSize_; ++i)
                    data_[i] += b.data_[i];
            };

            void sub(const self& b) {
                for (size_t i = 0; i < pSize_; ++i)
                    data_[i] -= b.data_[i];
            };

            template <typename U>
            void scale(U factor) {
                for (size_t i = 0; i < pSize_; ++i)
                    data_[i] *= factor;
            }

            scalar_type dot(const self& b) const {
                scalar_type rt{};

                for (size_t i = 0; i < pSize_; ++i)
                    rt += data_[i] * b.data_[i];

                return rt;
            }

            scalar_type sqLenght() const {
                return dot(*this);
            }

            auto lenght() -> typename lenght_type<scalar_type>::type const {
                return std::sqrt(sqLenght());
            }

            void noramlize() {
                auto len = lenght();

                // check len != 0

                for (auto& elem : data_) {
                    elem /= len;
                }
            }

            void inverse() {
                for (auto& elem : data_) {
                    elem = -elem;
                }
            }

            friend self operator+(const self& lhs, const self& rhs) {
                auto rt = lhs;
                rt.sum(rhs);
                return rt;
            }

            self& operator+=(const self& rhs) {
                (*this).sum(rhs);
                return *this;
            }

            friend self operator-(const self& lhs, const self& rhs) {
                auto rt = lhs;
                rt.sub(rhs);
                return rt;
            }

            self& operator-=(const self& rhs) {
                (*this).sub(rhs);
                return *this;
            }

            template <typename U>
            friend self operator*(const self& lhs, U factor) {
                auto rt = lhs;
                rt.scale(factor);
                return rt;
            }

            template <typename U>
            self& operator*=(U factor) {
                (*this).scale(factor);
                return *this;
            }

            friend self operator*(const self& lhs, const self& rhs) {
                auto rt = *lhs;
                rt.dot(rhs);
                return rt;
            }

            self& operator*=(const self& rhs) {
                (*this).dot(rhs);
                return *this;
            }

            [[nodiscard]] pointer data() {
                return data_.data();
            }

            [[nodiscard]] const_pointer data() const {
                return data_.data();
            }

            std::array<T, pSize_> data_;
    };

    template <typename T>
    struct vector2 : vector_base<vector2, T, __SZ2> {
            using self = vector2<T>;
            using scalar_type = T;
            using reference = T&;
            using pointer = T*;
            using const_pointer = const T*;

            static constexpr size_t size = __SZ2;

            using vector_base<vector2, T, __SZ2>::data_;

            [[nodiscard]]
            scalar_type x() const {
                return data_[0];
            }

            [[nodiscard]]
            scalar_type y() const {
                return data_[1];
            }

            friend self operator+(const self& lhs, const self& rhs) {
                auto rt = lhs;
                rt.sum(rhs);
                return rt;
            }

            self& operator+=(const self& rhs) {
                (*this).sum(rhs);
                return *this;
            }

            friend self operator-(const self& lhs, const self& rhs) {
                auto rt = lhs;
                rt.sub(rhs);
                return rt;
            }

            self& operator-=(const self& rhs) {
                (*this).sub(rhs);
                return *this;
            }

            template <typename U>
            friend self operator*(const self& lhs, U factor) {
                auto rt = lhs;
                rt.scale(factor);
                return rt;
            }

            template <typename U>
            self& operator*=(U factor) {
                (*this).scale(factor);
                return *this;
            }

            friend self operator*(const self& lhs, const self& rhs) {
                auto rt = *lhs;
                rt.dot(rhs);
                return rt;
            }

            self& operator*=(const self& rhs) {
                (*this).dot(rhs);
                return *this;
            }

            [[nodiscard]] pointer data() {
                return data_.data();
            }

            [[nodiscard]] const_pointer data() const {
                return data_.data();
            }
    };

    // vector3
    template <typename T>
    struct vector3_base;

    export {
        template <typename T>
        vector3_base<T> sum(const vector3_base<T>& lhs, const vector3_base<T>& rhs) {
            return { lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2] };
        }

        template <typename T>
        vector3_base<T> sub(const vector3_base<T>& lhs, const vector3_base<T>& rhs) {
            return { lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2] };
        }

        template <typename T>
        vector3_base<T> scale(const vector3_base<T>& lhs, T factor) {
            return { lhs[0] * factor, lhs[1] * factor, lhs[2] * factor };
        }

        template <typename T>
        T dot(const vector3_base<T>& lhs, const vector3_base<T>& rhs) {
            return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
        }

        template <typename T>
        vector3_base<T> cross(const vector3_base<T>& lhs, const vector3_base<T>& rhs) {
            return { lhs[1] * rhs[2] - lhs[2] * rhs[1],
                     lhs[2] * rhs[0] - lhs[0] * rhs[2],
                     lhs[0] * rhs[1] - lhs[1] * rhs[0] };
        }
    }

    template <typename T>
    struct vector3_base {
            using self = vector3_base<T>;
            using scalar_type = T;
            using reference = T&;
            using pointer = T*;
            using const_pointer = const T*;

            static constexpr size_t size = __SZ3;

            vector3_base() = default;
            vector3_base(scalar_type x, scalar_type y, scalar_type z) : data_{ x, y, z } {
            }

            scalar_type& operator[](size_t id) {
                return data_[id];
            }

            const scalar_type operator[](size_t id) const {
                return data_[id];
            }

            [[nodiscard]]
            scalar_type x() const {
                return data_[0];
            }

            [[nodiscard]]
            scalar_type y() const {
                return data_[1];
            }

            [[nodiscard]]
            scalar_type z() const {
                return data_[2];
            }

            void sum(const self& rhs) {
                *this = tire::algebra::sum(*this, rhs);
            }

            friend self operator+(const self& lhs, const self& rhs) {
                return tire::algebra::sum(lhs, rhs);
            }

            self& operator+=(const self& rhs) {
                *this = tire::algebra::sum(*this, rhs);
                return *this;
            }

            friend self operator-(const self& lhs, const self& rhs) {
                return sub(lhs, rhs);
            }

            self& operator-=(const self& rhs) {
                *this = sub(*this, rhs);
                return *this;
            }

            template <typename U>
            friend self operator*(const self& lhs, U factor) {
                return scale(lhs, factor);
            }

            template <typename U>
            self& operator*=(U factor) {
                *this = scale(*this, factor);
                return *this;
            }

            [[nodiscard]] pointer data() {
                return data_.data();
            }

            [[nodiscard]] const_pointer data() const {
                return data_.data();
            }

        private:
            std::array<T, __SZ3> data_;
    };

    template <typename T>
    struct vector4_base;

    export {
        template <typename T>
        vector4_base<T> sum(const vector4_base<T>& lhs, const vector4_base<T>& rhs) {
            return { lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2], lhs[3] + rhs[3] };
        }

        template <typename T>
        vector4_base<T> sub(const vector4_base<T>& lhs, const vector4_base<T>& rhs) {
            return { lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2], lhs[3] - rhs[3] };
        }

        template <typename T>
        vector4_base<T> scale(const vector4_base<T>& lhs, T factor) {
            return { lhs[0] * factor, lhs[1] * factor, lhs[2] * factor, lhs[3] * factor };
        }

        template <typename T>
        T dot(const vector4_base<T>& lhs, const vector4_base<T>& rhs) {
            return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2] + lhs[3] * rhs[3];
        }
    }

    template <typename T>
    struct vector4_base {
            using self = vector4_base<T>;
            using scalar_type = T;
            using reference = T&;
            using pointer = T*;
            using const_pointer = const T*;

            static constexpr size_t size = __SZ4;

            vector4_base() = default;
            vector4_base(scalar_type x, scalar_type y, scalar_type z, scalar_type w)
                : data_{ x, y, z, w } {
            }

            [[nodiscard]] scalar_type x() const {
                return data_[0];
            }

            [[nodiscard]] scalar_type y() const {
                return data_[1];
            }

            [[nodiscard]] scalar_type z() const {
                return data_[2];
            }

            [[nodiscard]] scalar_type w() const {
                return data_[3];
            }

            void sum(const self& rhs) {
                *this = tire::algebra::sum(*this, rhs);
            }

            friend self operator+(const self& lhs, const self& rhs) {
                return tire::algebra::sum(lhs, rhs);
            }

            self& operator+=(const self& rhs) {
                *this = tire::algebra::sum(*this, rhs);
                return *this;
            }

            friend self operator-(const self& lhs, const self& rhs) {
                return sub(lhs, rhs);
            }

            self& operator-=(const self& rhs) {
                *this = sub(*this, rhs);
                return *this;
            }

            template <typename U>
            friend self operator*(const self& lhs, U factor) {
                return scale(lhs, factor);
            }

            template <typename U>
            self& operator*=(U factor) {
                *this = scale(*this, factor);
                return *this;
            }

            [[nodiscard]] pointer data() {
                return data_.data();
            }

            [[nodiscard]] const_pointer data() const {
                return data_.data();
            }

        private:
            std::array<T, __SZ4> data_;
    };

    template <typename T>
    struct dummy_derived {};

    export {
        template <typename T, size_t pSize_>
        using Vector = vector_base<dummy_derived, T, pSize_>;

        template <typename T>
        using Vector2 = vector2<T>;

        template <typename T>
        using Vector3 = vector3_base<T>;

        template <typename T>
        using Vector4 = vector4_base<T>;

        using Vector2i = Vector2<int>;
        using Vector3i = Vector3<int>;
        using Vector4i = Vector4<int>;

        using Vector2ui = Vector2<unsigned int>;
        using Vector3ui = Vector3<unsigned int>;
        using Vector4ui = Vector4<unsigned int>;

        using Vector2ll = Vector2<long long>;
        using Vector3ll = Vector3<long long>;
        using Vector4ll = Vector4<long long>;

        using Vector2f = Vector2<float>;
        using Vector3f = Vector3<float>;
        using Vector4f = Vector4<float>;

        using Vector2d = Vector2<double>;
        using Vector3d = Vector3<double>;
        using Vector4d = Vector4<double>;

        template <typename T>
        concept ConceptVector = std::is_same_v<T, Vector4ll> || std::is_same_v<T, Vector3f>
                                || std::is_same_v<T, Vector4d> || std::is_same_v<T, Vector4i>
                                || std::is_same_v<T, Vector4ui> || std::is_same_v<T, Vector3ll>
                                || std::is_same_v<T, Vector3f> || std::is_same_v<T, Vector3d>
                                || std::is_same_v<T, Vector3i> || std::is_same_v<T, Vector3ui>
                                || std::is_same_v<T, Vector2ll> || std::is_same_v<T, Vector2f>
                                || std::is_same_v<T, Vector2d> || std::is_same_v<T, Vector2i>
                                || std::is_same_v<T, Vector2ui>;
    }

}  // namespace tire::algebra

