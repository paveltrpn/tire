
#ifndef __algebravector_h__
#define __algebravector_h__

#include <cstddef>
#include <initializer_list>
#include <cmath>
#include <array>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/glm.hpp>

namespace tire {

static constexpr size_t __SZ2 = 2;
static constexpr size_t __SZ3 = 3;
static constexpr size_t __SZ4 = 4;

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
        using value_type = T;
        using reference = T&;
        using pointer = T*;

        value_type& operator[](size_t id) {
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

        value_type dot(const self& b) const {
            value_type rt{};

            for (size_t i = 0; i < pSize_; ++i)
                rt += data_[i] * b.data_[i];

            return rt;
        }

        value_type sqLenght() const {
            return dot(*this);
        }

        auto lenght() -> typename lenght_type<value_type>::type const {
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

        std::array<T, pSize_> data_;
};

template <typename T>
struct vector2 : vector_base<vector2, T, __SZ2> {
        using self = vector2<T>;
        using value_type = T;
        using reference = T&;
        using pointer = T*;

        using vector_base<vector2, T, __SZ2>::data_;

        [[nodiscard]]
        value_type x() const {
            return data_[0];
        }

        [[nodiscard]]
        value_type y() const {
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
};

// vector3
template <typename T>
struct vector3 : vector_base<vector3, T, __SZ3> {
        using self = vector3<T>;
        using value_type = T;
        using reference = T&;
        using pointer = T*;

        using vector_base<vector3, T, __SZ3>::data_;

        [[nodiscard]]
        value_type x() const {
            return data_[0];
        }

        [[nodiscard]]
        value_type y() const {
            return data_[1];
        }

        [[nodiscard]]
        value_type z() const {
            return data_[2];
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

        void cross(self b) {
            *this = { data_[1] * b[2] - data_[2] * b[1],
                      data_[2] * b[0] - data_[0] * b[2],
                      data_[0] * b[1] - data_[1] * b[0] };
        }

        self operator^(const self& rhs) {
            auto rt = *this;
            rt.cross(rhs);
            return rt;
        }
};

template <typename T>
struct vector4 : vector_base<vector4, T, __SZ4> {
        using self = vector4<T>;
        using value_type = T;
        using reference = T&;
        using pointer = T*;

        using vector_base<vector4, T, __SZ4>::data_;

        [[nodiscard]] value_type x() const {
            return data_[0];
        }

        [[nodiscard]] value_type y() const {
            return data_[1];
        }

        [[nodiscard]] value_type z() const {
            return data_[2];
        }

        [[nodiscard]] value_type w() const {
            return data_[3];
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
};

template <typename T>
struct dummy_derived {};

template <typename T, size_t pSize_>
using vector = vector_base<dummy_derived, T, pSize_>;

using vector2i = vector2<int>;
using vector3i = vector3<int>;
using vector4i = vector4<int>;

using vector2f = vector2<float>;
using vector3f = vector3<float>;
using vector4f = vector4<float>;

using vector2d = vector2<double>;
using vector3d = vector3<double>;
using vector4d = vector4<double>;

namespace algebra {

template <typename T, size_t size_>
struct vector_base_glm {
        using scalar_type = T;
        using self = glm::vec<size_, scalar_type>;

        self sum(const vector_base_glm& rhs) {
            return vec_ + rhs.vec_;
        }

        self sub(const vector_base_glm& rhs) {
            return vec_ - rhs.vec_;
        }

        self scale(scalar_type factor) {
            return vec_ * factor;
        }

        void sumSelf(const vector_base_glm& rhs) {
            vec_ += rhs.vec_;
        }

        void subSelf(const vector_base_glm& rhs) {
            vec_ -= rhs.vec_;
        }

        void scaleSelf(scalar_type factor) {
            vec_ *= factor;
        }

        scalar_type dot(const vector_base_glm& rhs) {
            return glm::dot(vec_, rhs.vec_);
        }

    private:
        glm::vec<size_, scalar_type> vec_;
};

template <typename T>
struct vector_base_glm<T, 2> {
        using scalar_type = T;
        using self = glm::vec<2, scalar_type>;

        vector_base_glm() = default;
        vector_base_glm(scalar_type x, scalar_type y) : vec_{ x, y } {};

        scalar_type& x() {
            return vec_[0];
        }

        scalar_type& y() {
            return vec_[1];
        }

        scalar_type x() const {
            return vec_[0];
        }

        scalar_type y() const {
            return vec_[1];
        }

        self sum(const vector_base_glm& rhs) {
            return vec_ + rhs.vec_;
        }

        self sub(const vector_base_glm& rhs) {
            return vec_ - rhs.vec_;
        }

        self scale(scalar_type factor) {
            return vec_ * factor;
        }

        void sumSelf(const vector_base_glm& rhs) {
            vec_ += rhs.vec_;
        }

        void subSelf(const vector_base_glm& rhs) {
            vec_ -= rhs.vec_;
        }

        void scaleSelf(scalar_type factor) {
            vec_ *= factor;
        }

        scalar_type dot(const vector_base_glm& rhs) {
            return glm::dot(vec_, rhs.vec_);
        }

        scalar_type length() {
            return vec_.length();
        }

        void normalize() {
            return glm::normalize(vec_);
        }

    private:
        glm::vec<2, scalar_type> vec_;
};

template <typename T>
struct vector_base_glm<T, 3> {
        using scalar_type = T;
        using self = glm::vec<3, scalar_type>;

        vector_base_glm() = default;
        vector_base_glm(scalar_type x, scalar_type y, scalar_type z) : vec_{ x, y, z } {};

        scalar_type& x() {
            return vec_[0];
        }

        scalar_type& y() {
            return vec_[1];
        }

        scalar_type& z() {
            return vec_[2];
        }

        scalar_type x() const {
            return vec_[0];
        }

        scalar_type y() const {
            return vec_[1];
        }

        scalar_type z() const {
            return vec_[2];
        }

        self sum(const vector_base_glm& rhs) {
            return vec_ + rhs.vec_;
        }

        self sub(const vector_base_glm& rhs) {
            return vec_ - rhs.vec_;
        }

        self scale(scalar_type factor) {
            return vec_ * factor;
        }

        void sumSelf(const vector_base_glm& rhs) {
            vec_ += rhs.vec_;
        }

        void subSelf(const vector_base_glm& rhs) {
            vec_ -= rhs.vec_;
        }

        void scaleSelf(scalar_type factor) {
            vec_ *= factor;
        }

        scalar_type dot(const vector_base_glm& rhs) {
            return glm::dot(vec_, rhs.vec_);
        }

        scalar_type length() {
            return vec_.length();
        }

        void normalize() {
            return glm::normalize(vec_);
        }

        self cross(const vector_base_glm& rhs) {
            return glm::cross(vec_, rhs.vec_);
        }

        self crossSelf(const vector_base_glm& rhs) {
            vec_ = glm::cross(vec_, rhs.vec_);
        }

    private:
        glm::vec<3, scalar_type> vec_;
};

template <typename T>
struct vector_base_glm<T, 4> {
        using scalar_type = T;
        using self = glm::vec<4, scalar_type>;

        vector_base_glm() = default;
        vector_base_glm(scalar_type x, scalar_type y, scalar_type z, scalar_type w)
            : vec_{ x, y, z, w } {};

        scalar_type& x() {
            return vec_[0];
        }

        scalar_type& y() {
            return vec_[1];
        }

        scalar_type& z() {
            return vec_[2];
        }

        scalar_type& w() {
            return vec_[3];
        }

        scalar_type x() const {
            return vec_[0];
        }

        scalar_type y() const {
            return vec_[1];
        }

        scalar_type z() const {
            return vec_[2];
        }

        scalar_type w() const {
            return vec_[3];
        }

        self sum(const vector_base_glm& rhs) {
            return vec_ + rhs.vec_;
        }

        self sub(const vector_base_glm& rhs) {
            return vec_ - rhs.vec_;
        }

        self scale(scalar_type factor) {
            return vec_ * factor;
        }

        void sumSelf(const vector_base_glm& rhs) {
            vec_ += rhs.vec_;
        }

        void subSelf(const vector_base_glm& rhs) {
            vec_ -= rhs.vec_;
        }

        void scaleSelf(scalar_type factor) {
            vec_ *= factor;
        }

        scalar_type dot(const vector_base_glm& rhs) {
            return glm::dot(vec_, rhs.vec_);
        }

        scalar_type length() {
            return vec_.length();
        }

        void normalize() {
            return glm::normalize(vec_);
        }

    private:
        glm::vec<4, scalar_type> vec_;
};

template <typename T, size_t size_>
using Vector = vector_base_glm<T, size_>;

template <typename T>
using Vector2 = Vector<T, 2>;

template <typename T>
using Vector3 = Vector<T, 3>;

template <typename T>
using Vector4 = Vector<T, 4>;

using Vector2i = Vector2<long long>;
using Vector3i = Vector3<long long>;
using Vector4i = Vector4<long long>;

using Vector2f = Vector2<float>;
using Vector3f = Vector3<float>;
using Vector4f = Vector4<float>;

using Vector2d = Vector2<double>;
using Vector3d = Vector3<double>;
using Vector4d = Vector4<double>;
}  // namespace algebra

}  // namespace tire

#endif
