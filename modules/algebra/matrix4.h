
#pragma once

#include <cstddef>
#include <initializer_list>
#include <cmath>
#include <concepts>
#include <numbers>

#include "matrix.h"
#include "vector3.h"
#include "vector4.h"

namespace tire::algebra
{

template <std::floating_point T> constexpr T _half_circle = T( 180.0L );

template <std::floating_point T> constexpr T degToRad( T deg ) {
    return deg * std::numbers::pi_v<T> / _half_circle<T>;
}

template <typename T> struct matrix4 final : public matrix_sqr_base<T, 4> {
    using base_type = matrix_sqr_base<T, 4>;
    using typename base_type::value_type;
    using self = matrix4<value_type>;
    // Introduce name _data from matrix_sqr_base namespace because of parent type is template.
    using base_type::_data;

    matrix4() { idtt(); }

    matrix4( std::initializer_list<value_type> list ) {
        for ( auto i = 0; const auto e : list ) {
            _data[i] = e;
            ++i;
        }
    }

    void zero() {
        _data[0] = T{};
        _data[1] = T{};
        _data[2] = T{};
        _data[3] = T{};
        _data[4] = T{};
        _data[5] = T{};
        _data[6] = T{};
        _data[7] = T{};
        _data[8] = T{};
        _data[9] = T{};
        _data[10] = T{};
        _data[11] = T{};
        _data[12] = T{};
        _data[13] = T{};
        _data[14] = T{};
        _data[15] = T{};
    }

    void idtt() {
        _data[0] = T{ 1 };
        _data[1] = T{};
        _data[2] = T{};
        _data[3] = T{};
        _data[4] = T{};
        _data[5] = T{ 1 };
        _data[6] = T{};
        _data[7] = T{};
        _data[8] = T{};
        _data[9] = T{};
        _data[10] = T{ 1 };
        _data[11] = T{};
        _data[12] = T{};
        _data[13] = T{};
        _data[14] = T{};
        _data[15] = T{ 1 };
    }

    void multiply( const self &rhs ) {
        auto this00 = ( *this )[0, 0];
        auto this01 = ( *this )[0, 1];
        auto this02 = ( *this )[0, 2];

        ( *this )[0, 0] = this00 * rhs[0, 0] + this01 * rhs[1, 0] + this02 * rhs[2, 0] + ( *this )[0, 3] * rhs[3, 0];
        ( *this )[0, 1] = this00 * rhs[0, 1] + this01 * rhs[1, 1] + this02 * rhs[2, 1] + ( *this )[0, 3] * rhs[3, 1];
        ( *this )[0, 2] = this00 * rhs[0, 2] + this01 * rhs[1, 2] + this02 * rhs[2, 2] + ( *this )[0, 3] * rhs[3, 2];
        ( *this )[0, 3] = this00 * rhs[0, 3] + this01 * rhs[1, 3] + this02 * rhs[2, 3] + ( *this )[0, 3] * rhs[3, 3];

        auto this10 = ( *this )[1, 0];
        auto this11 = ( *this )[1, 1];
        auto this12 = ( *this )[1, 2];

        ( *this )[1, 0] = this10 * rhs[0, 0] + this11 * rhs[1, 0] + this12 * rhs[2, 0] + ( *this )[1, 3] * rhs[3, 0];
        ( *this )[1, 1] = this10 * rhs[0, 1] + this11 * rhs[1, 1] + this12 * rhs[2, 1] + ( *this )[1, 3] * rhs[3, 1];
        ( *this )[1, 2] = this10 * rhs[0, 2] + this11 * rhs[1, 2] + this12 * rhs[2, 2] + ( *this )[1, 3] * rhs[3, 2];
        ( *this )[1, 3] = this10 * rhs[0, 3] + this11 * rhs[1, 3] + this12 * rhs[2, 3] + ( *this )[1, 3] * rhs[3, 3];

        auto this20 = ( *this )[2, 0];
        auto this21 = ( *this )[2, 1];
        auto this22 = ( *this )[2, 2];

        ( *this )[2, 0] = this20 * rhs[0, 0] + this21 * rhs[1, 0] + this22 * rhs[2, 0] + ( *this )[2, 3] * rhs[3, 0];
        ( *this )[2, 1] = this20 * rhs[0, 1] + this21 * rhs[1, 1] + this22 * rhs[2, 1] + ( *this )[2, 3] * rhs[3, 1];
        ( *this )[2, 2] = this20 * rhs[0, 2] + this21 * rhs[1, 2] + this22 * rhs[2, 2] + ( *this )[2, 3] * rhs[3, 2];
        ( *this )[2, 3] = this20 * rhs[0, 3] + this21 * rhs[1, 3] + this22 * rhs[2, 3] + ( *this )[2, 3] * rhs[3, 3];

        auto this30 = ( *this )[3, 0];
        auto this31 = ( *this )[3, 1];
        auto this32 = ( *this )[3, 2];

        ( *this )[3, 0] = this30 * rhs[0, 0] + this31 * rhs[1, 0] + this32 * rhs[2, 0] + ( *this )[3, 3] * rhs[3, 0];
        ( *this )[3, 1] = this30 * rhs[0, 1] + this31 * rhs[1, 1] + this32 * rhs[2, 1] + ( *this )[3, 3] * rhs[3, 1];
        ( *this )[3, 2] = this30 * rhs[0, 2] + this31 * rhs[1, 2] + this32 * rhs[2, 2] + ( *this )[3, 3] * rhs[3, 2];
        ( *this )[3, 3] = this30 * rhs[0, 3] + this31 * rhs[1, 3] + this32 * rhs[2, 3] + ( *this )[3, 3] * rhs[3, 3];
    }

    self operator*( const self &rhs ) {
        auto tmp{ *this };
        tmp.multiply( rhs );
        *this = tmp;
        return *this;
    }

    vector3<value_type> mult_vector3( const vector3<value_type> &v ) {
        vector3<value_type> rt;
        value_type w;

        rt[0] = v[0] * ( *this )[0] + v[1] * ( *this )[1] + v[2] * ( *this )[2] + ( *this )[3];
        rt[1] = v[0] * ( *this )[4] + v[1] * ( *this )[5] + v[2] * ( *this )[6] + ( *this )[7];
        rt[2] = v[0] * ( *this )[8] + v[1] * ( *this )[9] + v[2] * ( *this )[10] + ( *this )[11];
        w = v[0] * ( *this )[12] + v[1] * ( *this )[13] + v[2] * ( *this )[14] + ( *this )[15];

        // normalize if w is different than 1 (convert from homogeneous to Cartesian
        // coordinates)
        if ( w != 1.0f ) {
            rt[0] /= w;
            rt[1] /= w;
            rt[2] /= w;
        }

        return rt;
    }

    vector4<value_type> mult_vector4( const vector4<value_type> &v ) {
        vector4<value_type> rt;

        rt[0] = v[0] * ( *this )[0] + v[1] * ( *this )[1] + v[2] * ( *this )[2] + ( *this )[3];
        rt[1] = v[0] * ( *this )[4] + v[1] * ( *this )[5] + v[2] * ( *this )[6] + ( *this )[7];
        rt[2] = v[0] * ( *this )[8] + v[1] * ( *this )[9] + v[2] * ( *this )[10] + ( *this )[11];
        rt[3] = v[0] * ( *this )[12] + v[1] * ( *this )[13] + v[2] * ( *this )[14] + ( *this )[15];

        return rt;
    }

    value_type determinant() {}

    void rotation( value_type yaw, value_type pitch, value_type roll ) {
        auto siny = std::sin( yaw );
        auto cosy = std::cos( yaw );
        auto sinp = std::sin( pitch );
        auto cosp = std::cos( pitch );
        auto sinr = std::sin( roll );
        auto cosr = std::cos( roll );

        ( *this )[0] = cosy * cosr - siny * cosp * sinr;
        ( *this )[1] = -cosy * sinr - siny * cosp * cosr;
        ( *this )[2] = siny * sinp;
        ( *this )[3] = 0.0;

        ( *this )[4] = siny * cosr + cosy * cosp * sinr;
        ( *this )[5] = -siny * sinr + cosy * cosp * cosr;
        ( *this )[6] = -cosy * sinp;
        ( *this )[7] = 0.0;

        ( *this )[8] = sinp * sinr;
        ( *this )[9] = sinp * cosr;
        ( *this )[10] = cosp;
        ( *this )[11] = 0.0;

        ( *this )[12] = 0.0;
        ( *this )[13] = 0.0;
        ( *this )[14] = 0.0;
        ( *this )[15] = 1.0;
    }

    void perspective( value_type fovy, value_type aspect, value_type near, value_type far ) {
        value_type h =
            std::cos( static_cast<value_type>( 0.5 ) * fovy ) / std::sin( static_cast<value_type>( 0.5 ) * fovy );

        ( *this )[0] = h / aspect;
        ( *this )[1] = 0.0;
        ( *this )[2] = 0.0;
        ( *this )[3] = 0.0;
        ( *this )[4] = 0.0;
        ( *this )[5] = h;
        ( *this )[6] = 0.0;
        ( *this )[7] = 0.0;
        ( *this )[8] = 0.0;
        ( *this )[9] = 0.0;
        ( *this )[11] = -1.0;
        ( *this )[12] = 0.0;
        ( *this )[13] = 0.0;
        ( *this )[15] = 0.0;

        if ( far >= std::numeric_limits<value_type>::epsilon() ) {
            value_type nf;
            nf = 1.0 / ( far - near );
            ( *this )[10] = -( far + near ) * nf;
            ( *this )[14] = -( 2.0 * far * near ) * nf;
        } else {
            ( *this )[10] = -1.0;
            ( *this )[14] = -2.0 * near;
        }
    }

    void lookAt( const vector3<value_type> &eye, const vector3<value_type> &center, const vector3<value_type> &up ) {
        vector3<value_type> eyeDir;

        constexpr value_type floatEps = std::numeric_limits<value_type>::epsilon();
        if ( std::fabs( eye[0] - center[0] ) < floatEps && std::fabs( eye[1] - center[1] ) < floatEps &&
             std::fabs( eye[2] - center[2] ) < floatEps ) {
            return;
        }

        value_type z0 = eye[0] - center[0];
        value_type z1 = eye[1] - center[1];
        value_type z2 = eye[2] - center[2];

        value_type len = 1.0 / std::hypot( z0, z1, z2 );
        z0 *= len;
        z1 *= len;
        z2 *= len;

        value_type x0 = up[1] * z2 - up[2] * z1;
        value_type x1 = up[2] * z0 - up[0] * z2;
        value_type x2 = up[0] * z1 - up[1] * z0;
        len = std::hypot( x0, x1, x2 );
        if ( len == 0.0 ) {
            x0 = 0;
            x1 = 0;
            x2 = 0;
        } else {
            len = 1.0 / len;
            x0 *= len;
            x1 *= len;
            x2 *= len;
        }

        value_type y0 = z1 * x2 - z2 * x1;
        value_type y1 = z2 * x0 - z0 * x2;
        value_type y2 = z0 * x1 - z1 * x0;

        len = std::hypot( y0, y1, y2 );
        if ( len == 0.0 ) {
            y0 = 0;
            y1 = 0;
            y2 = 0;
        } else {
            len = 1.0 / len;
            y0 *= len;
            y1 *= len;
            y2 *= len;
        }

        ( *this )[0] = x0;
        ( *this )[1] = y0;
        ( *this )[2] = z0;
        ( *this )[3] = 0.0;
        ( *this )[4] = x1;
        ( *this )[5] = y1;
        ( *this )[6] = z1;
        ( *this )[7] = 0.0;
        ( *this )[8] = x2;
        ( *this )[9] = y2;
        ( *this )[10] = z2;
        ( *this )[11] = 0.0;
        ( *this )[12] = -( x0 * eye[0] + x1 * eye[1] + x2 * eye[2] );
        ( *this )[13] = -( y0 * eye[0] + y1 * eye[1] + y2 * eye[2] );
        ( *this )[14] = -( z0 * eye[0] + z1 * eye[1] + z2 * eye[2] );
        ( *this )[15] = 1.0;
    }

    void orthographic( value_type left, value_type right, value_type bottom, value_type top, value_type near,
                       value_type far ) {
        ( *this )[0] = 2.0 / ( right - left );
        ( *this )[1] = 0.0;
        ( *this )[2] = 0.0;
        ( *this )[3] = 0.0;

        ( *this )[4] = 0.0;
        ( *this )[5] = 2.0 / ( top - bottom );
        ( *this )[6] = 0.0;
        ( *this )[7] = 0.0;

        ( *this )[8] = 0.0;
        ( *this )[9] = 0.0;
        ( *this )[10] = -2.0 / ( far - near );
        ( *this )[11] = 0.0;

        ( *this )[12] = -( right + left ) / ( right - left );
        ( *this )[13] = -( top + bottom ) / ( top - bottom );
        ( *this )[14] = -( far + near ) / ( far - near );
        ( *this )[15] = 1.0;
    }

    void scale( const vector3<value_type> &offset ) {
        idtt();

        ( *this )[0] = offset[0];
        ( *this )[5] = offset[1];
        ( *this )[11] = offset[2];
    }

    void translate( const vector3<value_type> &offset ) {
        idtt();

        ( *this )[3] = offset[0];
        ( *this )[7] = offset[1];
        ( *this )[11] = offset[2];
    }

    void translate( value_type dx, value_type dy, value_type dz ) {
        idtt();

        ( *this )[3] = dx;
        ( *this )[7] = dy;
        ( *this )[11] = dz;
    }

    void rotation_yaw( value_type angl ) {
        idtt();
        value_type sa{}, ca{};

        auto a = degToRad( angl );
        sa = std::sin( a );
        ca = std::cos( a );

        ( *this )[5] = ca;
        ( *this )[6] = -sa;
        ( *this )[9] = sa;
        ( *this )[10] = ca;
    }

    void rotation_pitch( value_type angl ) {
        idtt();
        value_type sa{}, ca{};

        auto a = degToRad( angl );
        sa = std::sin( a );
        ca = std::cos( a );

        ( *this )[0] = ca;
        ( *this )[2] = -sa;
        ( *this )[8] = sa;
        ( *this )[10] = ca;
    }

    void rotation_roll( value_type angl ) {
        idtt();
        value_type sa{}, ca{};

        auto a = degToRad( angl );
        sa = std::sin( a );
        ca = std::cos( a );

        ( *this )[0] = ca;
        ( *this )[1] = -sa;
        ( *this )[4] = sa;
        ( *this )[5] = ca;
    }

    void euler( value_type yaw, value_type pitch, value_type roll ) {
        self y, p, r;

        y.rotation_yaw( yaw );
        p.rotation_pitch( pitch );
        r.rotation_roll( roll );

        *this = y * p * r;
    }

    void axis_angle( const vector3<value_type> &ax, value_type phi ) {
        value_type cosphi, sinphi, vxvy, vxvz, vyvz, vx, vy, vz;

        cosphi = std::cos( degToRad( phi ) );
        sinphi = std::sin( degToRad( phi ) );
        vxvy = ax[0] * ax[1];
        vxvz = ax[0] * ax[2];
        vyvz = ax[1] * ax[2];
        vx = ax[0];
        vy = ax[1];
        vz = ax[2];

        ( *this )[0] = cosphi + ( 1.0 - cosphi ) * vx * vx;
        ( *this )[1] = ( 1.0 - cosphi ) * vxvy - sinphi * vz;
        ( *this )[2] = ( 1.0 - cosphi ) * vxvz + sinphi * vy;
        ( *this )[3] = 0.0f;

        ( *this )[4] = ( 1.0 - cosphi ) * vxvy + sinphi * vz;
        ( *this )[5] = cosphi + ( 1.0 - cosphi ) * vy * vy;
        ( *this )[6] = ( 1.0 - cosphi ) * vyvz - sinphi * vx;
        ( *this )[7] = 0.0f;

        ( *this )[8] = ( 1.0 - cosphi ) * vxvz - sinphi * vy;
        ( *this )[9] = ( 1.0 - cosphi ) * vyvz + sinphi * vx;
        ( *this )[10] = cosphi + ( 1.0 - cosphi ) * vz * vz;
        ( *this )[11] = 0.0f;

        ( *this )[12] = 0.0f;
        ( *this )[13] = 0.0f;
        ( *this )[14] = 0.0f;
        ( *this )[15] = 1.0f;
    }
};

template <typename T> auto transpose( matrix4<T> &arg ) -> decltype( auto ) {
    matrix4<T> rt{ arg };
    rt.transpose();
    return rt;
}

template <typename T> matrix4<T> translate( T dx, T dy, T dz ) {
    matrix4<T> rt{};
    rt.translate( dx, dy, dz );
    return rt;
}

template <typename T> matrix4<T> translate( vector3<T> &offset ) {
    matrix4<T> rt{};
    rt.translate( offset.x(), offset.y(), offset.z() );
    return rt;
}

template <typename T> matrix4<T> rotate( T dy, T dp, T dr ) {
    matrix4<T> rt;
    rt.euler( dy, dp, dr );
    return rt;
}

template <typename T> matrix4<T> rotate( vector3<T> ax, T phi ) {
    matrix4<T> rt;
    rt.axis_angle( ax, phi );
    return rt;
}

template <typename T> matrix4<T> perspective( float fov, float aspect, float ncp, float fcp ) {
    matrix4<T> rt;
    rt.perspective( fov, aspect, ncp, fcp );
    return rt;
}

template <typename T> matrix4<T> orthographic( T left, T right, T bottom, T top, T near, T far ) {
    matrix4<T> rt;
    rt.orthographic( left, right, bottom, top, near, far );
    return rt;
}

using matrix4l = matrix4<long long>;
using matrix4f = matrix4<float>;
using matrix4d = matrix4<double>;

} // namespace tire::algebra
