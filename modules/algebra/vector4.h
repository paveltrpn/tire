
#pragma once

#include <cstddef>
#include <cmath>
#include <array>

#include "concepts.h"

namespace tire::algebra {

    template<typename T> requires Algebraic<T>
    struct vector4 final {
        using value_type = T;
        using self = vector4<value_type>;

        vector4() {
            data_[0] = value_type{};
            data_[1] = value_type{};
            data_[2] = value_type{};
            data_[3] = value_type{};
        }

        vector4(value_type x, value_type y, value_type z, value_type w) {
            data_[0] = x;
            data_[1] = y;
            data_[2] = z;
            data_[3] = w;
        }

        vector4(const std::array<value_type, 4> &arr) {
            data_[0] = arr[0];
            data_[1] = arr[1];
            data_[2] = arr[2];
            data_[3] = arr[3];
        }

        vector4(const self &rhs) {
            data_[0] = rhs.data_[0];
            data_[1] = rhs.data_[1];
            data_[2] = rhs.data_[2];
            data_[3] = rhs.data_[3];
        }

        value_type x() { return data_[0]; }

        value_type y() { return data_[1]; }

        value_type z() { return data_[2]; }

        value_type w() { return data_[3]; }

        void plus(const self &b) {
            for (size_t i = 0; i < 3; ++i) data_[i] += b.data_[i];
        };

        void minus(const self &b) {
            for (size_t i = 0; i < 3; ++i) data_[i] -= b.data_[i];
        };

        template<typename U>
        void scaleSelf(U factor) {
            data_[0] *= factor;
            data_[1] *= factor;
            data_[2] *= factor;
            data_[3] *= factor;
        }

        template<typename U>
        self scale(U factor) {
            const auto x = data_[0] * factor;
            const auto y = data_[1] * factor;
            const auto z = data_[2] * factor;
            const auto w = data_[3] * factor;
            return {value_type(x), value_type(y), value_type(z),
                    value_type(w)};
        }

        value_type dot(const self &b) const {
            value_type rt{};

            for (size_t i = 0; i < 3; ++i) rt += data_[i] * b.data_[i];

            return rt;
        }

        value_type sqLength() const { return dot(*this); }

        auto length() -> value_type const {
            // TODO: call proper square root function for float and double
            return std::sqrt(sqLength());
        }

        void normalizeSelf() {
            value_type len = length();

            if (len > 0.0) {
                for (size_t i{}; i < 3; ++i) {
                    data_[i] /= len;
                }
            }
        }

        friend self operator+(const self &lhs, const self &rhs) {
            auto rt = lhs;
            rt.plus(rhs);
            return rt;
        }

        self &operator+=(const self &rhs) {
            (*this).plus(rhs);
            return *this;
        }

        friend self operator-(const self &lhs, const self &rhs) {
            auto rt = lhs;
            rt.minus(rhs);
            return rt;
        }

        self &operator-=(const self &rhs) {
            (*this).minus(rhs);
            return *this;
        }

        value_type *data() { return data_.data(); }

        const value_type *data() const { return data_.data(); }

    private:
        std::array<value_type, 4> data_{};
    };

    using vector4l = vector4<long long>;
    using vector4f = vector4<float>;
    using vector4d = vector4<double>;

}  // namespace tire::algebra
