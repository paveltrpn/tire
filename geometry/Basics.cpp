
module;

export module geometry:Basics;

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

export template <toy::Arithmetical T>
struct point2 : basics2<point2, T> {
    using basics2<point2, T>::data_;
};

export template <toy::Arithmetical T>
struct point3 : basics3<point3, T> {
    using basics3<point3, T>::data_;
};

export template <toy::Arithmetical T>
struct normal3 : basics3<normal3, T> {
    using basics3<normal3, T>::data_;
};

};  // namespace tire
