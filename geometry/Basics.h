
#ifndef __gometry_basics_h__
#define __gometry_basics_h__

#include <cmath>
#include <limits>

import toy_std;
import toy_std.algebra;

namespace tire {

template <template <typename> typename derived, toy::Arithmetical T>
struct basics2 {
        toy::algebra::vector2<T> data_;
};

template <template <typename> typename derived, toy::Arithmetical T>
struct basics3 {
        toy::algebra::vector3<T> data_;
};

template <toy::Arithmetical T>
struct point2 : basics2<point2, T> {
        using basics2<point2, T>::data_;
};

template <toy::Arithmetical T>
struct point3 : basics3<point3, T> {
        using basics3<point3, T>::data_;
};

template <toy::Arithmetical T>
struct normal3 : basics3<normal3, T> {
        using basics3<normal3, T>::data_;
};

template <toy::Arithmetical T>
class OnTheLine {
        point3<T> const p1;
        point3<T> const p2;

    public:
        OnTheLine(point3<T> const &p1, point3<T> const &p2) : p1(p1), p2(p2) {
        }

        bool match(point3<T> const &p) const override {
            auto Tepsilon = std::numeric_limits<T>::epsilon();

            auto rx = (p2.x() - p1.x() >= Tepsilon) ? (p.x() - p1.x()) / (p2.x() - p1.x()) : T{};
            auto ry = (p2.y() - p1.y() >= Tepsilon) ? (p.y() - p1.y()) / (p2.y() - p1.y()) : T{};
            auto rz = (p2.z() - p1.z() >= Tepsilon) ? (p.z() - p1.z()) / (p2.z() - p1.z()) : T{};

            return (std::abs(rx - ry) <= Tepsilon) && (std::abs(ry - rz) <= Tepsilon)
                   && (std::abs(rx - rz) <= Tepsilon);
            // Approx(rx).epsilon(0.01) == ry && Approx(ry).epsilon(0.01) == rz;
        }
};

};  // namespace tire

#endif
