
module;

#include <cstddef>
#include <initializer_list>
#include <cmath>
#include <concepts>
#include <numbers>
#include <type_traits>
#include <array>

export module algebra:matrix4;

import :common;
import :concepts;
import :vector3;
import :vector4;

namespace tire::algebra {

export template <Algebraic T>
struct matrix4 final {
    using value_type = T;
    using self = matrix4<value_type>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    matrix4() { idtt(); }

    matrix4( std::initializer_list<value_type> list ) {
        for ( auto i = 0; const auto e : list ) {
            data_[i] = e;
            ++i;
        }
    }

    [[nodiscard]] reference operator[]( size_t index ) { return data_[index]; }

    [[nodiscard]] const_reference operator[]( size_t index ) const {
        return data_[index];
    }

    [[nodiscard]] size_t size() { return data_.size(); }

    [[nodiscard]] pointer data() { return data_.data(); }

    [[nodiscard]] const_pointer data() const { return data_.data(); }

    void zero() {
        data_[0] = T{};
        data_[1] = T{};
        data_[2] = T{};
        data_[3] = T{};
        data_[4] = T{};
        data_[5] = T{};
        data_[6] = T{};
        data_[7] = T{};
        data_[8] = T{};
        data_[9] = T{};
        data_[10] = T{};
        data_[11] = T{};
        data_[12] = T{};
        data_[13] = T{};
        data_[14] = T{};
        data_[15] = T{};
    }

    void idtt() {
        data_[0] = T{ 1 };
        data_[1] = T{};
        data_[2] = T{};
        data_[3] = T{};
        data_[4] = T{};
        data_[5] = T{ 1 };
        data_[6] = T{};
        data_[7] = T{};
        data_[8] = T{};
        data_[9] = T{};
        data_[10] = T{ 1 };
        data_[11] = T{};
        data_[12] = T{};
        data_[13] = T{};
        data_[14] = T{};
        data_[15] = T{ 1 };
    }

    [[nodiscard]] self transpose() const {
        /* value_type tmp;
        auto rt = *this;

        for ( size_t i = 0; i < 4; ++i ) {
            for ( size_t j = 0; j < i; ++j ) {
                tmp = rt[j, i];
                rt[j, i] = rt[i, j];
                rt[i, j] = tmp;
            }
        }

        return rt;*/
    }

    void transposeSelf() {
        /* value_type tmp;
        for ( size_t i = 0; i < 4; ++i ) {
            for ( size_t j = 0; j < i; ++j ) {
                tmp = ( *this )[j, i];
                ( *this )[j, i] = ( *this )[i, j];
                ( *this )[i, j] = tmp;
            }
        }*/

        for ( int i = 0; i < 4; ++i ) {
            for ( int j = i + 1; j < 4; ++j ) {
                int ij = i * 4 + j;
                int ji = j * 4 + i;
                std::swap( data_[ij], data_[ji] );
            }
        }
    }

    void multiply( const self& rhs ) {
        /*
        auto this00 = ( *this )[0, 0];
        auto this01 = ( *this )[0, 1];
        auto this02 = ( *this )[0, 2];

        ( *this )[0, 0] = this00 * rhs[0, 0] + this01 * rhs[1, 0] +
                          this02 * rhs[2, 0] + ( *this )[0, 3] * rhs[3, 0];
        ( *this )[0, 1] = this00 * rhs[0, 1] + this01 * rhs[1, 1] +
                          this02 * rhs[2, 1] + ( *this )[0, 3] * rhs[3, 1];
        ( *this )[0, 2] = this00 * rhs[0, 2] + this01 * rhs[1, 2] +
                          this02 * rhs[2, 2] + ( *this )[0, 3] * rhs[3, 2];
        ( *this )[0, 3] = this00 * rhs[0, 3] + this01 * rhs[1, 3] +
                          this02 * rhs[2, 3] + ( *this )[0, 3] * rhs[3, 3];

        auto this10 = ( *this )[1, 0];
        auto this11 = ( *this )[1, 1];
        auto this12 = ( *this )[1, 2];

        ( *this )[1, 0] = this10 * rhs[0, 0] + this11 * rhs[1, 0] +
                          this12 * rhs[2, 0] + ( *this )[1, 3] * rhs[3, 0];
        ( *this )[1, 1] = this10 * rhs[0, 1] + this11 * rhs[1, 1] +
                          this12 * rhs[2, 1] + ( *this )[1, 3] * rhs[3, 1];
        ( *this )[1, 2] = this10 * rhs[0, 2] + this11 * rhs[1, 2] +
                          this12 * rhs[2, 2] + ( *this )[1, 3] * rhs[3, 2];
        ( *this )[1, 3] = this10 * rhs[0, 3] + this11 * rhs[1, 3] +
                          this12 * rhs[2, 3] + ( *this )[1, 3] * rhs[3, 3];

        auto this20 = ( *this )[2, 0];
        auto this21 = ( *this )[2, 1];
        auto this22 = ( *this )[2, 2];

        ( *this )[2, 0] = this20 * rhs[0, 0] + this21 * rhs[1, 0] +
                          this22 * rhs[2, 0] + ( *this )[2, 3] * rhs[3, 0];
        ( *this )[2, 1] = this20 * rhs[0, 1] + this21 * rhs[1, 1] +
                          this22 * rhs[2, 1] + ( *this )[2, 3] * rhs[3, 1];
        ( *this )[2, 2] = this20 * rhs[0, 2] + this21 * rhs[1, 2] +
                          this22 * rhs[2, 2] + ( *this )[2, 3] * rhs[3, 2];
        ( *this )[2, 3] = this20 * rhs[0, 3] + this21 * rhs[1, 3] +
                          this22 * rhs[2, 3] + ( *this )[2, 3] * rhs[3, 3];

        auto this30 = ( *this )[3, 0];
        auto this31 = ( *this )[3, 1];
        auto this32 = ( *this )[3, 2];

        ( *this )[3, 0] = this30 * rhs[0, 0] + this31 * rhs[1, 0] +
                          this32 * rhs[2, 0] + ( *this )[3, 3] * rhs[3, 0];
        ( *this )[3, 1] = this30 * rhs[0, 1] + this31 * rhs[1, 1] +
                          this32 * rhs[2, 1] + ( *this )[3, 3] * rhs[3, 1];
        ( *this )[3, 2] = this30 * rhs[0, 2] + this31 * rhs[1, 2] +
                          this32 * rhs[2, 2] + ( *this )[3, 3] * rhs[3, 2];
        ( *this )[3, 3] = this30 * rhs[0, 3] + this31 * rhs[1, 3] +
                          this32 * rhs[2, 3] + ( *this )[3, 3] * rhs[3, 3];*/
        std::array<T, 16> temp;

        for ( int i = 0; i < 4; ++i ) {
            for ( int j = 0; j < 4; ++j ) {
                temp[i * 4 + j] = 0;
                for ( int k = 0; k < 4; ++k ) {
                    temp[i * 4 + j] += data_[i * 4 + k] * rhs[k * 4 + j];
                }
            }
        }

        for ( int i = 0; i < 16; ++i ) {
            data_[i] = temp[i];
        }
    }

    self& operator*( const self& rhs ) {
        this->multiply( rhs );
        return *this;
    }

    vector3<value_type> mult_vector3( const vector3<value_type>& v ) const {
        value_type w{};

        auto rx = v.x() * ( *this )[0] + v.y() * ( *this )[1] +
                  v.z() * ( *this )[2] + ( *this )[3];
        auto ry = v.x() * ( *this )[4] + v.y() * ( *this )[5] +
                  v.z() * ( *this )[6] + ( *this )[7];
        auto rz = v.x() * ( *this )[8] + v.y() * ( *this )[9] +
                  v.z() * ( *this )[10] + ( *this )[11];
        w = v.x() * ( *this )[12] + v.y() * ( *this )[13] +
            v.z() * ( *this )[14] + ( *this )[15];

        // normalize if w is different than 1 (convert from homogeneous to Cartesian
        // coordinates)
        if ( w != 1.0f ) {
            rx /= w;
            ry /= w;
            rz /= w;
        }

        return { value_type( rx ), value_type( ry ), value_type( rz ) };
    }

    vector4<value_type> mult_vector4( const vector4<value_type>& v ) const {
        vector4<value_type> rt;

        rt[0] = v[0] * ( *this )[0] + v[1] * ( *this )[1] +
                v[2] * ( *this )[2] + ( *this )[3];
        rt[1] = v[0] * ( *this )[4] + v[1] * ( *this )[5] +
                v[2] * ( *this )[6] + ( *this )[7];
        rt[2] = v[0] * ( *this )[8] + v[1] * ( *this )[9] +
                v[2] * ( *this )[10] + ( *this )[11];
        rt[3] = v[0] * ( *this )[12] + v[1] * ( *this )[13] +
                v[2] * ( *this )[14] + ( *this )[15];

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

        data_[0] = cosy * cosr - siny * cosp * sinr;
        data_[1] = -cosy * sinr - siny * cosp * cosr;
        data_[2] = siny * sinp;
        data_[3] = 0.0;

        data_[4] = siny * cosr + cosy * cosp * sinr;
        data_[5] = -siny * sinr + cosy * cosp * cosr;
        data_[6] = -cosy * sinp;
        data_[7] = 0.0;

        data_[8] = sinp * sinr;
        data_[9] = sinp * cosr;
        data_[10] = cosp;
        data_[11] = 0.0;

        data_[12] = 0.0;
        data_[13] = 0.0;
        data_[14] = 0.0;
        data_[15] = 1.0;
    }

    void perspective( value_type fov, value_type aspect, value_type near,
                      value_type far ) {
        const auto radFov = degToRad( fov );

        // NOTE: GNU related function sincos()
        // Choose sincos() function according to value_type.
        value_type sinFov{}, cosFov{};
        if constexpr ( std::is_same_v<value_type, double> ) {
            sincos( static_cast<value_type>( 0.5 ) * radFov, &sinFov, &cosFov );
        } else if constexpr ( std::is_same_v<value_type, float> ) {
            sincosf( static_cast<value_type>( 0.5 ) * radFov, &sinFov,
                     &cosFov );
        }

        // value_type tanFov =
        // std::tan( static_cast<value_type>( 0.5 ) * radFov );
        value_type tanFov = sinFov / cosFov;
        value_type focal = static_cast<value_type>( 1.0 ) / tanFov;

        data_[0] = focal / aspect;
        data_[1] = value_type{ 0 };
        data_[2] = value_type{ 0 };
        data_[3] = value_type{ 0 };
        data_[4] = value_type{ 0 };
        data_[5] = focal;
        data_[6] = value_type{ 0 };
        data_[7] = value_type{ 0 };
        data_[8] = value_type{ 0 };
        data_[9] = value_type{ 0 };
        data_[11] = value_type{ -1 };
        data_[12] = value_type{ 0 };
        data_[13] = value_type{ 0 };
        data_[15] = value_type{ 0 };

        if ( far >= std::numeric_limits<value_type>::epsilon() ) {
            value_type nf{ static_cast<value_type>( 1.0 ) / ( far - near ) };
            data_[10] = -( far + near ) * nf;
            data_[14] = -( 2.0 * far * near ) * nf;
        } else {
            data_[10] = value_type{ -1 };
            data_[14] = -2.0 * near;
        }
    }

    void vperspective( value_type fov, value_type aspect, value_type near,
                       value_type far ) {
        const auto radFov = degToRad( fov );

        // NOTE: GNU related function sincos()
        // Choose sincos() function according to value_type.

        value_type sinFov{}, cosFov{};
        if constexpr ( std::is_same_v<value_type, double> ) {
            sincos( static_cast<value_type>( 0.5 ) * radFov, &sinFov, &cosFov );
        } else if constexpr ( std::is_same_v<value_type, float> ) {
            sincosf( static_cast<value_type>( 0.5 ) * radFov, &sinFov,
                     &cosFov );
        }

        // value_type tanFov = std::tan( value_type{ 0.5 } * radFov );
        value_type tanFov = sinFov / cosFov;
        value_type focal = 1.0 / tanFov;
        value_type nf{ near / ( far - near ) };

        data_[0] = focal / aspect;
        data_[4] = value_type{ 0 };
        data_[8] = value_type{ 0 };
        data_[12] = value_type{ 0 };

        data_[1] = value_type{ 0 };
        data_[5] = -focal;
        data_[9] = value_type{ 0 };
        data_[13] = value_type{ 0 };

        data_[2] = value_type{ 0 };
        data_[6] = value_type{ 0 };
        data_[10] = nf;
        data_[14] = far * nf;

        data_[3] = value_type{ 0 };
        data_[7] = value_type{ 0 };
        data_[11] = value_type{ -1 };
        data_[15] = value_type{ 0 };

        // TODO: Check and write in transpose version.
        /*
            const matrix4<value_type> inverse;
            inverse[0] = aspect / focal;
            inverse[1] = value_type{ 0 };
            inverse[2] = value_type{ 0 };
            inverse[3] = value_type{ 0 };

            inverse[4] = value_type{ 0 };
            inverse[5] = 1 / -focal;
            inverse[6] = value_type{ 0 };
            inverse[7] = value_type{ 0 };

            inverse[8] = value_type{ 0 };
            inverse[9] = value_type{ 0 };
            inverse[10] = value_type{ 0 };
            inverse[11] = value_type{ -1 };

            inverse[12] = value_type{ 0 };
            inverse[13] = value_type{ 0 };
            inverse[14] = 1 / ( far * nf );
            inverse[15] = nf / ( far * nf );
            */
    }

    void vperspective2( value_type fov, value_type aspect, value_type near,
                        value_type far ) {
        const auto radFov = degToRad( fov );

        // NOTE: GNU related function sincos()
        // Choose sincos() function according to value_type.

        value_type sinFov{}, cosFov{};
        if constexpr ( std::is_same_v<value_type, double> ) {
            sincos( static_cast<value_type>( 0.5 ) * radFov, &sinFov, &cosFov );
        } else if constexpr ( std::is_same_v<value_type, float> ) {
            sincosf( static_cast<value_type>( 0.5 ) * radFov, &sinFov,
                     &cosFov );
        }

        // value_type tanFov = std::tan( value_type{ 0.5 } * radFov );
        value_type tanFov = sinFov / cosFov;
        value_type focal = static_cast<value_type>( 1.0 ) / tanFov;
        value_type nf{ far / ( far - near ) };

        data_[0] = focal / aspect;
        data_[1] = value_type{ 0 };
        data_[2] = value_type{ 0 };
        data_[3] = value_type{ 0 };

        data_[4] = value_type{ 0 };
        data_[5] = focal;
        data_[6] = value_type{ 0 };
        data_[7] = value_type{ 0 };

        data_[8] = value_type{ 0 };
        data_[9] = value_type{ 0 };
        data_[10] = nf;
        data_[11] = -near * nf;

        data_[12] = value_type{ 0 };
        data_[13] = value_type{ 0 };
        data_[14] = value_type{ 1 };
        data_[15] = value_type{ 0 };
    }

    void lookAt( const vector3<value_type>& eye,
                 const vector3<value_type>& target,
                 const vector3<value_type>& up ) {
        vector3<value_type> eyeDir;

        constexpr value_type floatEps =
            std::numeric_limits<value_type>::epsilon();
        if ( std::fabs( eye[0] - target[0] ) < floatEps &&
             std::fabs( eye[1] - target[1] ) < floatEps &&
             std::fabs( eye[2] - target[2] ) < floatEps ) {
            return;
        }

        value_type z0 = eye[0] - target[0];
        value_type z1 = eye[1] - target[1];
        value_type z2 = eye[2] - target[2];

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

        data_[0] = x0;
        data_[1] = y0;
        data_[2] = z0;
        data_[3] = 0.0;
        data_[4] = x1;
        data_[5] = y1;
        data_[6] = z1;
        data_[7] = 0.0;
        data_[8] = x2;
        data_[9] = y2;
        data_[10] = z2;
        data_[11] = 0.0;
        data_[12] = -( x0 * eye[0] + x1 * eye[1] + x2 * eye[2] );
        data_[13] = -( y0 * eye[0] + y1 * eye[1] + y2 * eye[2] );
        data_[14] = -( z0 * eye[0] + z1 * eye[1] + z2 * eye[2] );
        data_[15] = 1.0;
    }

    void orthographic( value_type left, value_type right, value_type bottom,
                       value_type top, value_type near, value_type far ) {
        data_[0] = 2.0 / ( right - left );
        data_[1] = 0.0;
        data_[2] = 0.0;
        data_[3] = 0.0;

        data_[4] = 0.0;
        data_[5] = 2.0 / ( top - bottom );
        data_[6] = 0.0;
        data_[7] = 0.0;

        data_[8] = 0.0;
        data_[9] = 0.0;
        data_[10] = -2.0 / ( far - near );
        data_[11] = 0.0;

        data_[12] = -( right + left ) / ( right - left );
        data_[13] = -( top + bottom ) / ( top - bottom );
        data_[14] = -( far + near ) / ( far - near );
        data_[15] = 1.0;
    }

    void scale( const vector3<value_type>& offset ) {
        idtt();

        data_[0] = offset.x();
        data_[5] = offset.y();
        data_[10] = offset.z();
    }

    void translate( const vector3<value_type>& offset ) {
        idtt();

        data_[3] = offset.x();
        data_[7] = offset.y();
        data_[11] = offset.z();
    }

    void translate( value_type dx, value_type dy, value_type dz ) {
        idtt();

        data_[3] = dx;
        data_[7] = dy;
        data_[11] = dz;
    }

    void rotation_yaw( value_type angl ) {
        idtt();

        value_type sa{}, ca{};

        auto a = degToRad( angl );

        if constexpr ( std::is_same_v<value_type, double> ) {
            sincos( a, &sa, &ca );
        } else if constexpr ( std::is_same_v<value_type, float> ) {
            sincosf( a, &sa, &ca );
        }

        data_[5] = ca;
        data_[6] = -sa;
        data_[9] = sa;
        data_[10] = ca;
    }

    void rotation_pitch( value_type angl ) {
        idtt();

        value_type sa{}, ca{};

        auto a = degToRad( angl );

        if constexpr ( std::is_same_v<value_type, double> ) {
            sincos( a, &sa, &ca );
        } else if constexpr ( std::is_same_v<value_type, float> ) {
            sincosf( a, &sa, &ca );
        }

        data_[0] = ca;
        data_[2] = sa;
        data_[8] = -sa;
        data_[10] = ca;
    }

    void rotation_roll( value_type angl ) {
        idtt();

        value_type sa{}, ca{};

        auto a = degToRad( angl );

        if constexpr ( std::is_same_v<value_type, double> ) {
            sincos( a, &sa, &ca );
        } else if constexpr ( std::is_same_v<value_type, float> ) {
            sincosf( a, &sa, &ca );
        }

        data_[0] = ca;
        data_[1] = -sa;
        data_[4] = sa;
        data_[5] = ca;
    }

    void euler( value_type yaw, value_type pitch, value_type roll ) {
        self y, p, r;

        y.rotation_yaw( yaw );
        p.rotation_pitch( pitch );
        r.rotation_roll( roll );

        *this = y * p * r;
    }

    void axis_angle( const vector3<value_type>& ax, value_type phi ) {
        value_type cosphi, sinphi, vxvy, vxvz, vyvz, vx, vy, vz;

        cosphi = std::cos( degToRad( phi ) );
        sinphi = std::sin( degToRad( phi ) );
        vxvy = ax.x() * ax.y();
        vxvz = ax.x() * ax.z();
        vyvz = ax.y() * ax.z();
        vx = ax.x();
        vy = ax.y();
        vz = ax.z();

        data_[0] = cosphi + ( 1.0 - cosphi ) * vx * vx;
        data_[1] = ( 1.0 - cosphi ) * vxvy - sinphi * vz;
        data_[2] = ( 1.0 - cosphi ) * vxvz + sinphi * vy;
        data_[3] = 0.0f;

        data_[4] = ( 1.0 - cosphi ) * vxvy + sinphi * vz;
        data_[5] = cosphi + ( 1.0 - cosphi ) * vy * vy;
        data_[6] = ( 1.0 - cosphi ) * vyvz - sinphi * vx;
        data_[7] = 0.0f;

        data_[8] = ( 1.0 - cosphi ) * vxvz - sinphi * vy;
        data_[9] = ( 1.0 - cosphi ) * vyvz + sinphi * vx;
        data_[10] = cosphi + ( 1.0 - cosphi ) * vz * vz;
        data_[11] = 0.0f;

        data_[12] = 0.0f;
        data_[13] = 0.0f;
        data_[14] = 0.0f;
        data_[15] = 1.0f;
    }

    self inverse() const {
        self inv;
        value_type det;

        inv[0] =
            data_[5] * data_[10] * data_[15] -
            data_[5] * data_[11] * data_[14] - data_[9] * data_[6] * data_[15] +
            data_[9] * data_[7] * data_[14] + data_[13] * data_[6] * data_[11] -
            data_[13] * data_[7] * data_[10];

        inv[4] =
            -data_[4] * data_[10] * data_[15] +
            data_[4] * data_[11] * data_[14] + data_[8] * data_[6] * data_[15] -
            data_[8] * data_[7] * data_[14] - data_[12] * data_[6] * data_[11] +
            data_[12] * data_[7] * data_[10];

        inv[8] =
            data_[4] * data_[9] * data_[15] - data_[4] * data_[11] * data_[13] -
            data_[8] * data_[5] * data_[15] + data_[8] * data_[7] * data_[13] +
            data_[12] * data_[5] * data_[11] - data_[12] * data_[7] * data_[9];

        inv[12] =
            -data_[4] * data_[9] * data_[14] +
            data_[4] * data_[10] * data_[13] + data_[8] * data_[5] * data_[14] -
            data_[8] * data_[6] * data_[13] - data_[12] * data_[5] * data_[10] +
            data_[12] * data_[6] * data_[9];

        inv[1] =
            -data_[1] * data_[10] * data_[15] +
            data_[1] * data_[11] * data_[14] + data_[9] * data_[2] * data_[15] -
            data_[9] * data_[3] * data_[14] - data_[13] * data_[2] * data_[11] +
            data_[13] * data_[3] * data_[10];

        inv[5] =
            data_[0] * data_[10] * data_[15] -
            data_[0] * data_[11] * data_[14] - data_[8] * data_[2] * data_[15] +
            data_[8] * data_[3] * data_[14] + data_[12] * data_[2] * data_[11] -
            data_[12] * data_[3] * data_[10];

        inv[9] =
            -data_[0] * data_[9] * data_[15] +
            data_[0] * data_[11] * data_[13] + data_[8] * data_[1] * data_[15] -
            data_[8] * data_[3] * data_[13] - data_[12] * data_[1] * data_[11] +
            data_[12] * data_[3] * data_[9];

        inv[13] =
            data_[0] * data_[9] * data_[14] - data_[0] * data_[10] * data_[13] -
            data_[8] * data_[1] * data_[14] + data_[8] * data_[2] * data_[13] +
            data_[12] * data_[1] * data_[10] - data_[12] * data_[2] * data_[9];

        inv[2] =
            data_[1] * data_[6] * data_[15] - data_[1] * data_[7] * data_[14] -
            data_[5] * data_[2] * data_[15] + data_[5] * data_[3] * data_[14] +
            data_[13] * data_[2] * data_[7] - data_[13] * data_[3] * data_[6];

        inv[6] =
            -data_[0] * data_[6] * data_[15] + data_[0] * data_[7] * data_[14] +
            data_[4] * data_[2] * data_[15] - data_[4] * data_[3] * data_[14] -
            data_[12] * data_[2] * data_[7] + data_[12] * data_[3] * data_[6];

        inv[10] =
            data_[0] * data_[5] * data_[15] - data_[0] * data_[7] * data_[13] -
            data_[4] * data_[1] * data_[15] + data_[4] * data_[3] * data_[13] +
            data_[12] * data_[1] * data_[7] - data_[12] * data_[3] * data_[5];

        inv[14] =
            -data_[0] * data_[5] * data_[14] + data_[0] * data_[6] * data_[13] +
            data_[4] * data_[1] * data_[14] - data_[4] * data_[2] * data_[13] -
            data_[12] * data_[1] * data_[6] + data_[12] * data_[2] * data_[5];

        inv[3] =
            -data_[1] * data_[6] * data_[11] + data_[1] * data_[7] * data_[10] +
            data_[5] * data_[2] * data_[11] - data_[5] * data_[3] * data_[10] -
            data_[9] * data_[2] * data_[7] + data_[9] * data_[3] * data_[6];

        inv[7] =
            data_[0] * data_[6] * data_[11] - data_[0] * data_[7] * data_[10] -
            data_[4] * data_[2] * data_[11] + data_[4] * data_[3] * data_[10] +
            data_[8] * data_[2] * data_[7] - data_[8] * data_[3] * data_[6];

        inv[11] =
            -data_[0] * data_[5] * data_[11] + data_[0] * data_[7] * data_[9] +
            data_[4] * data_[1] * data_[11] - data_[4] * data_[3] * data_[9] -
            data_[8] * data_[1] * data_[7] + data_[8] * data_[3] * data_[5];

        inv[15] =
            data_[0] * data_[5] * data_[10] - data_[0] * data_[6] * data_[9] -
            data_[4] * data_[1] * data_[10] + data_[4] * data_[2] * data_[9] +
            data_[8] * data_[1] * data_[6] - data_[8] * data_[2] * data_[5];

        det = data_[0] * inv[0] + data_[1] * inv[4] + data_[2] * inv[8] +
              data_[3] * inv[12];

        if ( det == 0 ) return {};
        det = 1.0 / det;
        for ( int i = 0; i < 16; i++ ) inv[i] *= det;

        return inv;
    }

private:
    std::array<T, 16> data_{};
};

export {
    template <typename T>
    auto transpose( matrix4<T>& arg ) -> decltype( auto ) {
        matrix4<T> rt{ arg };
        rt.transpose();
        return rt;
    }

    template <typename T>
    matrix4<T> translate( T dx, T dy, T dz ) {
        matrix4<T> rt{};
        rt.translate( dx, dy, dz );
        return rt;
    }

    template <typename T>
    matrix4<T> translate( vector3<T>& offset ) {
        matrix4<T> rt{};
        rt.translate( offset );
        return rt;
    }

    template <typename T>
    matrix4<T> rotate( T dy, T dp, T dr ) {
        matrix4<T> rt;
        rt.euler( dy, dp, dr );
        return rt;
    }

    template <typename T>
    matrix4<T> rotate( vector3<T> ax, T phi ) {
        matrix4<T> rt;
        rt.axis_angle( ax, phi );
        return rt;
    }

    template <typename T>
    matrix4<T> scale( T dx, T dy, T dz ) {
        matrix4<T> rt;
        rt.scale( { dx, dy, dz } );
        return rt;
    }

    template <typename T>
    matrix4<T> perspective( float fov, float aspect, float ncp, float fcp ) {
        matrix4<T> rt;
        rt.perspective( fov, aspect, ncp, fcp );
        return rt;
    }

    template <typename T>
    matrix4<T> vperspective( float fov, float aspect, float ncp, float fcp ) {
        matrix4<T> rt;
        rt.vperspective( fov, aspect, ncp, fcp );
        return rt;
    }

    template <typename T>
    matrix4<T> vperspective2( float fov, float aspect, float ncp, float fcp ) {
        matrix4<T> rt;
        rt.vperspective2( fov, aspect, ncp, fcp );
        return rt;
    }

    template <typename T>
    matrix4<T> orthographic( T left, T right, T bottom, T top, T near, T far ) {
        matrix4<T> rt;
        rt.orthographic( left, right, bottom, top, near, far );
        return rt;
    }

    using matrix4l = matrix4<long long>;
    using matrix4f = matrix4<float>;
    using matrix4d = matrix4<double>;
}

}  // namespace tire::algebra
