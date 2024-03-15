
#include <cstddef>

#include "Vector.h"

namespace tire {

template <typename T>
struct quaternion {
        quaternion() = default;

        quaternion(quaternion&& rhs) = delete;

        explicit quaternion(const quaternion& rhs) : data_{ rhs.data_ } {
        }

        explicit quaternion(const std::array<T, 4>& rhs) {
            data_[0] = rhs[0];
            data_[1] = rhs[1];
            data_[2] = rhs[2];
            data_[3] = rhs[3];
        }

        quaternion(T x, T y, T z, T w) {
            data_[0] = x;
            data_[1] = y;
            data_[2] = z;
            data_[3] = w;
        }

        quaternion(const vector2<T>& rhs) {
            data_[0] = rhs.data_[0];
            data_[1] = rhs.data_[1];
            data_[2] = data_[3] = T{};
        }

        quaternion(const vector3<T>& rhs) {
            data_[0] = rhs.data_[0];
            data_[1] = rhs.data_[1];
            data_[2] = rhs.data_[2];
            data_[3] = T{};
        }

        explicit quaternion(const vector4<T>& rhs) {
            data_[0] = rhs.data_[0];
            data_[1] = rhs.data_[1];
            data_[2] = rhs.data_[2];
            data_[3] = rhs.data_[3];
        }

        T operator[](size_t id) {
            return data_[id];
        }

        const T operator[](size_t id) const {
            return data_[id];
        }

        T x() const {
            return data_[0];
        }

        T y() const {
            return data_[1];
        }

        T z() const {
            return data_[2];
        }

        T w() const {
            return data_[3];
        }

        quaternion& operator=(const quaternion& rhs) {
            if (this != &rhs) {
                data_[0] = rhs.data_[0];
                data_[1] = rhs.data_[1];
                data_[2] = rhs.data_[2];
                data_[3] = rhs.data_[3];
            }

            return *this;
        };

        quaternion operator+(const quaternion& b) {
            return quaternion{ data_[0] + b[0], data_[1] + b[1], data_[2] + b[2], data_[3] + b[3] };
        }

        quaternion operator+=(const quaternion& b) {
            return this + b;
        }

        quaternion operator-(const quaternion& b) {
            return quaternion{ data_[0] - b[0], data_[1] - b[1], data_[2] - b[2], data_[3] - b[3] };
        }

        quaternion operator-=(const quaternion& b) {
            return this - b;
        }

        quaternion& operator=(quaternion&& rhs) = delete;

        ~quaternion() = default;

        /*
         * Test purpose members
         */

        [[nodiscard]] T* getData() {
            return data_.data();
        };

    private:
        std::array<T, 4> data_{};
};

}  // namespace toy::algebra

