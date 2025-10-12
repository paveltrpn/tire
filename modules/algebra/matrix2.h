
#pragma once

#include <cstddef>
#include <initializer_list>
#include <mdspan>
#include "concepts.h"

namespace tire::algebra {

    template<typename T> requires Algebraic<T>
    struct matrix2 final {
        using value_type = T;
        using self = matrix2<value_type>;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;

        matrix2() {
            data_[0] = T{1};
            data_[1] = T{};
            data_[2] = T{};
            data_[3] = T{1};
        }

        matrix2(std::initializer_list<value_type> list) {
            for (auto i = 0; const auto e: list) {
                data_[i] = e;
                ++i;
            }
        }

        // row-wise indexing operator
        [[nodiscard]] reference operator[](size_t i, size_t j) {
            return std::mdspan(data_.data(), 2, 2)[i, j];
        }

        [[nodiscard]] const_reference operator[](size_t i, size_t j) const {
            return std::mdspan(data_.data(), 2, 2)[i, j];
        }

        [[nodiscard]] reference operator[](size_t index) { return data_[index]; }

        [[nodiscard]] const_reference operator[](size_t index) const {
            return data_[index];
        }

        [[nodiscard]] size_t size() { return data_.size(); }

        [[nodiscard]] pointer data() { return data_.data(); }

        [[nodiscard]] const_pointer data() const { return data_.data(); }

        [[nodiscard]] self transpose() {
            value_type tmp;
            auto rt = *this;

            for (size_t i = 0; i < 2; ++i) {
                for (size_t j = 0; j < i; ++j) {
                    tmp = rt[j, i];
                    rt[j, i] = rt[i, j];
                    rt[i, j] = tmp;
                }
            }

            return rt;
        }

        void transposeSelf() {
            value_type tmp;
            for (size_t i = 0; i < 2; ++i) {
                for (size_t j = 0; j < i; ++j) {
                    tmp = (*this)[j, i];
                    (*this)[j, i] = (*this)[i, j];
                    (*this)[i, j] = tmp;
                }
            }
        }

        void multiply(const self &rhs) {
            auto data0 = data_[0];
            auto data1 = data_[1];
            data_[0] = rhs[0] * data_[0] + rhs[1] * data_[2];
            data_[1] = rhs[0] * data_[1] + rhs[1] * data_[3];
            data_[2] = rhs[2] * data0 + rhs[3] * data_[2];
            data_[3] = rhs[2] * data1 + rhs[3] * data_[3];
        }

        [[nodiscard]] self operator*(const self &rhs) {
            auto tmp{*this};
            tmp.multiply(rhs);
            *this = tmp;
            return *this;
        }

        value_type determinant() {
            return data_[0] * data_[3] - data_[2] * data_[1];
        }

    private:
        std::array<value_type, 4> data_;
    };

    using matrix2l = matrix2<long long>;
    using matrix2f = matrix2<float>;
    using matrix2d = matrix2<double>;

}  // namespace tire::algebra
