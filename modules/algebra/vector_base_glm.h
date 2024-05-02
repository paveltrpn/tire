
#ifndef __vector_base_glm_h__
#define __vector_base_glm_h__

#include <glm/glm.hpp>

namespace tire::algebra {

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
}  // namespace tire::algebra

#endif
