
#ifndef __algebravectorbase_h__
#define __algebravectorbase_h__

#include <cstddef>
#include <array>

namespace tire {

template <template <typename> class derived_, typename T>
struct _vector2_base {
        using self = _vector2_base<derived_, T>;
        using derived = derived_<T>;

        template <typename U>
        auto operator+(derived_<U> rhs) -> derived_<decltype(T{} + U{})> {
            return { data_[0] + rhs.data_[0], data_[1] + rhs.data_[1] };
        }

        template <typename U>
        auto operator-(derived_<U> rhs) -> derived_<decltype(T{} - U{})> {
            return { data_[0] - rhs.data_[0], data_[1] - rhs.data_[1] };
        }

        template <typename U>
        auto operator*(derived_<U> rhs) -> derived_<decltype(T{} * U{})> {
            return { data_[0] * rhs.data_[0], data_[1] * rhs.data_[1] };
        }

        template <typename U>
        auto operator/(derived_<U> rhs) -> derived_<decltype(T{} / U{})> {
            return { data_[0] / rhs.data_[0], data_[1] / rhs.data_[1] };
        }

        template <typename U>
        derived_<T> &operator-=(derived_<U> rhs) {
            data_[0] -= rhs.data_[0];
            data_[1] -= rhs.data_[1];
            return static_cast<derived_<T> &>(*this);
        }

        template <typename U>
        derived_<T> &operator+=(derived_<U> rhs) {
            data_[0] += rhs.data_[0];
            data_[1] += rhs.data_[1];
            return static_cast<derived_<T> &>(*this);
        }

        template <typename U>
        derived_<T> &operator*=(derived_<U> rhs) {
            data_[0] *= rhs.data_[0];
            data_[1] *= rhs.data_[1];
            return static_cast<derived_<T> &>(*this);
        }

        template <typename U>
        derived_<T> &operator/=(derived_<U> rhs) {
            data_[0] /= rhs.data_[0];
            data_[1] /= rhs.data_[1];
            return static_cast<derived_<T> &>(*this);
        }

        std::array<T, 2> data_;
};

template <template <typename> class derived_, typename T>
struct _vector3_base {
        using self = _vector3_base<derived_, T>;
        using derived = derived_<T>;

        template <typename U>
        auto operator+(derived_<U> rhs) -> derived_<decltype(T{} + U{})> {
            return { data_[0] + rhs.data_[0], data_[1] + rhs.data_[1], data_[2] + rhs.data_[2] };
        }

        template <typename U>
        auto operator-(derived_<U> rhs) -> derived_<decltype(T{} - U{})> {
            return { data_[0] - rhs.data_[0], data_[1] - rhs.data_[1], data_[2] + rhs.data_[2] };
        }

        template <typename U>
        auto operator*(derived_<U> rhs) -> derived_<decltype(T{} * U{})> {
            return { data_[0] * rhs.data_[0], data_[1] * rhs.data_[1], data_[2] * rhs.data_[2] };
        }

        template <typename U>
        auto operator/(derived_<U> rhs) -> derived_<decltype(T{} / U{})> {
            return { data_[0] / rhs.data_[0], data_[1] / rhs.data_[1], data_[2] + rhs.data_[2] };
        }

        template <typename U>
        derived_<T> &operator-=(derived_<U> rhs) {
            data_[0] -= rhs.data_[0];
            data_[1] -= rhs.data_[1];
            data_[2] -= rhs.data_[2];
            return static_cast<derived_<T> &>(*this);
        }

        template <typename U>
        derived_<T> &operator+=(derived_<U> rhs) {
            data_[0] += rhs.data_[0];
            data_[1] += rhs.data_[1];
            data_[2] += rhs.data_[2];
            return static_cast<derived_<T> &>(*this);
        }

        template <typename U>
        derived_<T> &operator*=(derived_<U> rhs) {
            data_[0] *= rhs.data_[0];
            data_[1] *= rhs.data_[1];
            data_[2] *= rhs.data_[2];
            return static_cast<derived_<T> &>(*this);
        }

        template <typename U>
        derived_<T> &operator/=(derived_<U> rhs) {
            data_[0] /= rhs.data_[0];
            data_[1] /= rhs.data_[1];
            data_[2] /= rhs.data_[2];
            return static_cast<derived_<T> &>(*this);
        }

        std::array<T, 3> data_;
};

template <template <typename> class derived_, typename T>
struct _vector4_base {
        using self = _vector4_base<derived_, T>;
        using derived = derived_<T>;

        template <typename U>
        auto operator+(derived_<U> rhs) -> derived_<decltype(T{} + U{})> {
            return { data_[0] + rhs.data_[0], data_[1] + rhs.data_[1], data_[2] + rhs.data_[2], data_[3] + rhs.data_[3] };
        }

        template <typename U>
        auto operator-(derived_<U> rhs) -> derived_<decltype(T{} - U{})> {
            return { data_[0] - rhs.data_[0], data_[1] - rhs.data_[1], data_[2] + rhs.data_[2], data_[3] - rhs.data_[3] };
        }

        template <typename U>
        auto operator*(derived_<U> rhs) -> derived_<decltype(T{} * U{})> {
            return { data_[0] * rhs.data_[0], data_[1] * rhs.data_[1], data_[2] * rhs.data_[2], data_[3] * rhs.data_[3] };
        }

        template <typename U>
        auto operator/(derived_<U> rhs) -> derived_<decltype(T{} / U{})> {
            return { data_[0] / rhs.data_[0], data_[1] / rhs.data_[1], data_[2] + rhs.data_[2], data_[3] / rhs.data_[3] };
        }

        template <typename U>
        derived_<T> &operator-=(derived_<U> rhs) {
            data_[0] -= rhs.data_[0];
            data_[1] -= rhs.data_[1];
            data_[2] -= rhs.data_[2];
            data_[3] -= rhs.data_[3];
            return static_cast<derived_<T> &>(*this);
        }

        template <typename U>
        derived_<T> &operator+=(derived_<U> rhs) {
            data_[0] += rhs.data_[0];
            data_[1] += rhs.data_[1];
            data_[2] += rhs.data_[2];
            data_[3] += rhs.data_[3];
            return static_cast<derived_<T> &>(*this);
        }

        template <typename U>
        derived_<T> &operator*=(derived_<U> rhs) {
            data_[0] *= rhs.data_[0];
            data_[1] *= rhs.data_[1];
            data_[2] *= rhs.data_[2];
            data_[3] *= rhs.data_[3];
            return static_cast<derived_<T> &>(*this);
        }

        template <typename U>
        derived_<T> &operator/=(derived_<U> rhs) {
            data_[0] /= rhs.data_[0];
            data_[1] /= rhs.data_[1];
            data_[2] /= rhs.data_[2];
            data_[3] /= rhs.data_[3];
            return static_cast<derived_<T> &>(*this);
        }

        std::array<T, 4> data_;
};

template <typename T>
struct vector2 : tire::_vector2_base<vector2, T> {
        using tire::_vector2_base<vector2, T>::data_;

        T operator[](size_t id) {
            return data_[id];
        }
};

template <typename T>
struct vector3 : tire::_vector3_base<vector3, T> {
        using tire::_vector3_base<vector3, T>::data_;

        T operator[](size_t id) {
            return data_[id];
        }
};

template <typename T>
struct vector4 : tire::_vector4_base<vector4, T> {
        using tire::_vector4_base<vector4, T>::data_;

        T operator[](size_t id) {
            return data_[id];
        }
};

using vector2f = vector2<float>;
using vector3f = vector3<float>;
using vector4f = vector4<float>;

using vector2d = vector2<double>;
using vector3d = vector3<double>;
using vector4d = vector4<double>;

using vector2i = vector2<long long>;
using vector3i = vector3<long long>;
using vector4i = vector4<long long>;

}  // namespace tire

#endif